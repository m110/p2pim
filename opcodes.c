#include <stdlib.h>
#include <assert.h>
#include "net.h"
#include "opcodes.h"

#ifdef CLIENT
#include "opcodes_client.h"
#else
#include "opcodes_server.h"
#endif

int (*opcode_actions[OPCODES_COUNT]) (struct opcode_context *ctx);

char* status_messages[] = {
    "Success",                      // 0x000 STATUS_SUCCESS
    "Unknown error!",               // 0x001 ERROR_UNKNOWN
    "Registration required.",       // 0x001 ERROR_NOT_REGISTERED
    "Peer ID already exists."       // 0x003 ERROR_PEER_ID_EXISTS
};

inline int handle_opcode(struct opcode_context *ctx) {
    assert(ctx != NULL);
    return (*opcode_actions[ctx->p_ctx->opcode])(ctx);
}
