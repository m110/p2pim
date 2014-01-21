#ifndef _OPCODES_SERVER_H
#define _OPCODES_SERVER_H

#include "structs_server.h"

struct OpcodeData {
    Opcode opcode;
    char *message;
    Node *list;
    Node *node;
    Client *client;
};

#endif
