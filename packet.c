#include "packet.h"

/**
 * Prepares packet_context structure to be sent over the net.
 * You are responsible for freeing the message buffer in case of message_len > 0.
 */
int prepare_packet_full(struct packet_context *p_ctx, enum opcode opcode,
        enum opcode_status status, char *message, size_t message_len) {
    assert(message_len <= MAX_MESSAGE_LEN);

    p_ctx->opcode = opcode;
    p_ctx->status = status;

    if (message_len > 0) {
        p_ctx->message = malloc(message_len * sizeof(char));
        memcpy(p_ctx->message, message, message_len);
    } else {
        message_len = 1;
        p_ctx->message = malloc(sizeof(char));
        p_ctx->message[0] = '\0';
    }

    p_ctx->message_len = message_len;

    return 0;
}

/**
 * Prepares packet_context structure with opcode_status set to STATUS_SUCCESS.
 * Most cases should call this function instead of full prepare.
 */
int prepare_packet(struct packet_context *p_ctx, enum opcode opcode, char *message, size_t message_len) {
    return prepare_packet_full(p_ctx, opcode, STATUS_SUCCESS, message, message_len);
}

/**
 * Prepares packet_context structure with opcode_status set to STATUS_SUCCESS.
 * Additionally message_len is set to message length + 1 (for null byte).
 * This function should be called in most simple cases, when there are no null bytes
 * in message (except for the one at the end of course).
 */
int prepare_packet_message(struct packet_context *p_ctx, enum opcode opcode, char *message) {
    return prepare_packet(p_ctx, opcode, message, strlen(message)+1);
}

/**
 * Prepares packet_context structure with chosen opcode and opcode_status set to STATUS_SUCCESS.
 * The message will be set as empty.
 */
int prepare_packet_simple(struct packet_context *p_ctx, enum opcode opcode) {
    return prepare_packet(p_ctx, opcode, NULL, 0);
}

/**
 * Prepares packet_context structure with opcode COMMON_STATUS and chosen opcode_status.
 * The message will be set as empty.
 */
int prepare_packet_status(struct packet_context *p_ctx, enum opcode_status status) {
    return prepare_packet_full(p_ctx, COMMON_STATUS, status,  NULL, 0);
}

void print_packet(char *prefix, struct packet_context *p_ctx) {
    printf("%s[opcode: %d status: %d message: <%s> (len: %d)]", prefix,
            p_ctx->opcode, p_ctx->status, p_ctx->message, p_ctx->message_len);

    if (p_ctx->opcode == COMMON_STATUS) {
        printf(", \"%s\"", status_messages[p_ctx->status]);
    }

    printf("\n");
}
void print_packet_dump(struct packet_context *p_ctx) { int i; for (i = 0; i < (int) p_ctx->message_len; i++) {
        printf("%c", p_ctx->message[i]);
    }

    printf("\n");
}

/* Send packet to peer */
int packet_send(int socket, struct peer *peer, struct packet_context *p_ctx) {
    int bytes;
    char *data;
    char *message_tmp;
    size_t data_size;
    tpl_node *packet;
    tpl_bin message_bin;

    /* Pack message to message_bin */
    message_tmp = p_ctx->message;
    message_bin.addr = &message_tmp;
    message_bin.sz = p_ctx->message_len;

    /* This should be free'd after sending.
     * Set message to empty string to avoid mismatches.
     */
    p_ctx->message = NULL;

    /* Pack the packet */
    packet = tpl_map(PACKET_TPL_FORMAT, p_ctx, &message_bin);
    tpl_pack(packet, 0);
    tpl_dump(packet, TPL_MEM, &data, &data_size);
    tpl_free(packet);

    /* Preparing packet allocates this buffer, so free it when it's packed  */
    free(message_tmp);

    /* Send packed data through UDP */
    bytes = udp_send(socket, &peer->sockaddr, data, data_size);

    /* Free buffer allocated by TPL */
    free(data);

    return bytes;
}

/* Receive packet from peer */
int packet_recv(int socket, struct peer *peer, struct packet_context *p_ctx) {
    int bytes;
    char data[MAX_PACKET_SIZE];
    tpl_node *packet;
    tpl_bin message_bin;
    struct sockaddr_storage sockaddr;

    char address[INET6_ADDRSTRLEN];
    unsigned short port;

    /* Get data through UDP and save peer's net location */
    bytes = udp_recv(socket, (struct sockaddr *) &sockaddr, &data);
    get_address((struct sockaddr *) &sockaddr, address);
    port = get_port((struct sockaddr *) &sockaddr);

    /* Unpack the data to context structure */
    packet = tpl_map(PACKET_TPL_FORMAT, p_ctx, &message_bin);
    tpl_load(packet, TPL_MEM|TPL_PREALLOCD, data, bytes);
    tpl_unpack(packet, 0);
    tpl_free(packet);

    /* Save the message binary buffer to context */
    if (p_ctx->message_len <= MAX_MESSAGE_LEN) {
        p_ctx->message = malloc(p_ctx->message_len * sizeof(char));
        memcpy(p_ctx->message, *((char**) message_bin.addr), p_ctx->message_len);
    }

    /* Free buffer allocated by TPL */
    free(message_bin.addr);

    *peer = create_peer("temporary", address, port, (struct sockaddr *) &sockaddr);

    return bytes;
}

/* Send and receive packet */
int packet_dialog(int socket, struct peer *peer, struct packet_context *p_ctx) {
    struct peer dummy;
    packet_send(socket, peer, p_ctx);
    return packet_recv(socket, &dummy, p_ctx);
}


