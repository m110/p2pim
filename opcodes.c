#include "opcodes.h"

int (*opcode_actions[OPCODES_COUNT]) (char *message, void* arg);

char* StatusMessages[] = {
    "Success",                      // 0x000 STATUS_SUCCESS
    "Registration required.",       // 0x001 ERROR_NOT_REGISTERED 
    "Client ID already exists."     // 0x002 ERROR_CLIENT_ID_EXISTS
};

inline int handle_opcode(Opcode opcode, char *message, void* arg) {
    return (*opcode_actions[opcode])(message, arg);
}
