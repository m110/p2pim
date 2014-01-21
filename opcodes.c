#include <stdlib.h>
#include <assert.h>
#include "opcodes.h"

#ifdef CLIENT
#include "opcodes_client.h"
#else
#include "opcodes_server.h"
#endif

int (*opcode_actions[OPCODES_COUNT]) (struct opcode_context *ctx);

char* status_messages[] = {
    "Success",                      // 0x000 STATUS_SUCCESS
    "Registration required.",       // 0x001 ERROR_NOT_REGISTERED 
    "Client ID already exists."     // 0x002 ERROR_CLIENT_ID_EXISTS
};

inline int handle_opcode(struct opcode_context *ctx) {
    assert(ctx != NULL);
    return (*opcode_actions[ctx->opcode])(ctx);
}
