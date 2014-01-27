#include "p2pim.h"
#include "opcodes_client.h"

int handle_list(struct opcode_context *ctx) {
    tpl_node *packet;
    char *peer_id = NULL, *peer_address = NULL;
    unsigned short peer_port = 0;

    packet = tpl_map(LIST_TPL_FORMAT, &peer_id, &peer_address, &peer_port);
    tpl_load(packet, TPL_MEM|TPL_PREALLOCD|TPL_EXCESS_OK, ctx->p_ctx->message, ctx->p_ctx->message_len);
    while (tpl_unpack(packet, 1) > 0) {
        printf("Read: %s %s %d\n", peer_id, peer_address, peer_port);
    }
    tpl_free(packet);

    return RESPONSE_HANDLED;
}

void register_opcodes() {
    register_opcodes_common();
    opcode_actions[SERVER_LIST] = handle_list;
}
