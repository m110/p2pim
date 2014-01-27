#include "p2pim.h"
#include "opcodes.h"
#include "opcodes_server.h"
#include "net.h"
#include "include/tpl.h"

int handle_register(struct opcode_context *ctx) {
    struct peer *peer = ctx->peer;
    assert(peer != NULL);

    /* Set new ID, because it's set to 'temporary' */
    strncpy(peer->id, ctx->p_ctx->message, MAX_PEER_ID_LEN);

    if (get_node_by_id(*ctx->list, peer->id) == NULL) {
        printf("Register user %s from %s:%d\n",
                peer->id, 
                peer->public_addr.address, 
                peer->public_addr.port);

        return add_node(ctx->list, peer);
    } else {
        return ERROR_PEER_ID_EXISTS;
    }

    return STATUS_SUCCESS;
}

int handle_heartbeat(struct opcode_context *ctx) {
    struct node *node = ctx->node;
    if (node == NULL) {
        return ERROR_NOT_REGISTERED;
    }

    node->time = current_time();
    return 0;
}

int handle_close(struct opcode_context *ctx) {
    struct node *node = ctx->node;
    if (node == NULL) {
        return ERROR_NOT_REGISTERED;
    }

    delete_node(ctx->list, node);
    return 0;
}

int handle_list(struct opcode_context *ctx) {
    struct node *node = ctx->node;
    if (node == NULL) {
        return ERROR_NOT_REGISTERED;
    }

    struct packet_context p_ctx;
    char *data;
    size_t data_size;
    tpl_node *packet;

    /* List data */
    char *peer_id = NULL, *peer_address = NULL;
    unsigned short peer_port = 0;

    /* Pack the list */
    packet = tpl_map(LIST_TPL_FORMAT, &peer_id, &peer_address, &peer_port);
    for (struct node *n = *ctx->list; n != NULL; n = n->next) {
        struct peer *peer = &n->peer;
        // TODO check timeout
        peer_id = peer->id;
        peer_address = peer->public_addr.address;
        peer_port = peer->public_addr.port;
        tpl_pack(packet, 1);
    }
    tpl_dump(packet, TPL_MEM, &data, &data_size);
    tpl_free(packet);

    /* Prepare and send the list */
    prepare_packet(&p_ctx, SERVER_LIST, data);
    packet_send(ctx->socket, ctx->peer, &p_ctx);

    /* Free buffer allocated by TPL */
    free(data);

    return RESPONSE_HANDLED;
}

void register_opcodes() {
    register_opcodes_common();
    opcode_actions[CLIENT_REGISTER] = handle_register;
    opcode_actions[CLIENT_HEARTBEAT] = handle_heartbeat;
    opcode_actions[CLIENT_CLOSE] = handle_close;
    opcode_actions[CLIENT_LIST] = handle_list;
}
