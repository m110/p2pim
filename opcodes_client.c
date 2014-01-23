#include "p2pim.h"
#include "structs_common.h"
#include "opcodes.h"
#include "opcodes_client.h"

int handle_info(struct opcode_context *ctx) {
    return ctx->p_ctx->opcode;
}

void register_opcodes() {
    opcode_actions[SRV_INFO] = handle_info;
}
