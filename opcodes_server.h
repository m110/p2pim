#ifndef _OPCODES_SERVER_H
#define _OPCODES_SERVER_H

#include "structs_server.h"

struct opcode_context {
    enum opcode opcode;
    char *message;
    struct node *list;
    struct node *node;
    struct client *client;
};

#endif
