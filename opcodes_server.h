#ifndef _OPCODES_SERVER_H
#define _OPCODES_SERVER_H

#include "structs_server.h"

struct opcode_context {
    int socket;
    struct packet_context *p_ctx;
    struct node **list;
    struct peer *peer;
    struct node *node;
};

#endif
