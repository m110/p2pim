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
    p_ctx->message_len = message_len;

    if (message_len > 0) {
        // XXX p_ctx->message = malloc(message_len * sizeof(char));
        // XXX strncpy(p_ctx->message, message, message_len);
        strncpy(p_ctx->message, message, MAX_MESSAGE_LEN);
    } else {
        // XXX p_ctx->message = NULL;
        memset(p_ctx->message, 0, MAX_MESSAGE_LEN);
    }

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

/* Send packet to peer */
int packet_send(int socket, struct peer *peer, struct packet_context *p_ctx) {
    char *data;
    size_t data_size;
    tpl_node *packet;
    int bytes;

    /* Pack the packet */
    packet = tpl_map(PACKET_TPL_FORMAT, p_ctx, MAX_MESSAGE_LEN);
    tpl_pack(packet, 0);
    tpl_dump(packet, TPL_MEM, &data, &data_size);
    tpl_free(packet);

    /* Preparing packet allocates this buffer, so free it when it's packed  */
    // XXX free(p_ctx->message);

    /* Send packed data through UDP */
    bytes = udp_send(socket, &peer->sockaddr, data, data_size);

    /* Free buffer allocated by TPL */
    free(data);

    return bytes;
}

/* Receive packet from peer */
int packet_recv(int socket, struct peer *peer, struct packet_context *p_ctx) {
    char data[MAX_PACKET_SIZE];
    tpl_node *packet;
    struct sockaddr_storage sockaddr;
    int bytes;

    char address[INET6_ADDRSTRLEN];
    unsigned short port;

    /* Get data through UDP and save peer's net location */
    bytes = udp_recv(socket, (struct sockaddr *) &sockaddr, &data);
    get_address((struct sockaddr *) &sockaddr, address);
    port = get_port((struct sockaddr *) &sockaddr);

    /* Unpack the data to context structure */
    packet = tpl_map(PACKET_TPL_FORMAT, p_ctx, MAX_MESSAGE_LEN);
    tpl_load(packet, TPL_MEM|TPL_PREALLOCD, data, bytes);
    tpl_unpack(packet, 0);
    tpl_free(packet);

    *peer = create_peer("temporary", address, port, (struct sockaddr *) &sockaddr);

    return bytes;
}

/* Send and receive packet */
int packet_dialog(int socket, struct peer *peer, struct packet_context *p_ctx) {
    struct peer dummy;
    packet_send(socket, peer, p_ctx);
    return packet_recv(socket, &dummy, p_ctx);
}


