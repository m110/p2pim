#include <stdlib.h>
#include <assert.h>
#include "net.h"
#include "packet.h"
#include "opcodes.h"

#ifdef CLIENT
#include "opcodes_client.h"
#else
#include "opcodes_server.h"
#endif

int (*opcode_actions[OPCODES_COUNT]) (struct opcode_context *ctx);

char* status_messages[] = {
    "Success.",                     // 0x000 STATUS_SUCCESS
    "Unknown error.",               // 0x001 ERROR_UNKNOWN
    "Registration required.",       // 0x001 ERROR_NOT_REGISTERED
    "Peer ID already exists."       // 0x003 ERROR_PEER_ID_EXISTS
};

inline int handle_opcode(struct opcode_context *ctx) {
    assert(ctx != NULL);
    assert(ctx->p_ctx != NULL);
    assert(opcode_actions[ctx->p_ctx->opcode] != NULL);
    int result = (*opcode_actions[ctx->p_ctx->opcode])(ctx);

    /* Message is allocated by packet_recv, free it now */
    free(ctx->p_ctx->message);

    return result;
}

int handle_status(struct opcode_context *ctx) {
    print_packet("handle_status: ", ctx->p_ctx);
    return 0;
}

void register_opcodes_common() {
    opcode_actions[COMMON_STATUS] = handle_status;
}
