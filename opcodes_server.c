#include "p2pim.h"
#include "opcodes.h"
#include "opcodes_server.h"

int handle_register(struct opcode_context *ctx) {
    struct peer *peer = ctx->peer;
    assert(peer != NULL);

    if (get_node_by_id(ctx->list, peer->id) == NULL) {
        printf("Register user %s from %s:%d\n",
                peer->id, 
                peer->public_addr.address, 
                peer->public_addr.port);

        return add_node(&ctx->list, peer);
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

    delete_node(&ctx->list, node);
    return 0;
}

int handle_list(struct opcode_context *ctx) {
    struct node *node = ctx->node;
    if (node == NULL) {
        return ERROR_NOT_REGISTERED;
    }

    return 0;
}

void register_opcodes() {
    opcode_actions[CLI_REGISTER] = handle_register;
    opcode_actions[CLI_HEARTBEAT] = handle_heartbeat;
    opcode_actions[CLI_CLOSE] = handle_close;
    opcode_actions[CLI_LIST] = handle_list;
}
