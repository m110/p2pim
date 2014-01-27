#ifndef _OPCODES_SERVER_H
#define _OPCODES_SERVER_H

#include "structs_server.h"
#include "net.h"
#include "opcodes.h"
#include "packet.h"
#include "include/tpl.h"

struct opcode_context {
    int socket;
    struct packet_context *p_ctx;
    struct node **list;
    struct peer *peer;
    struct node *node;
};

#endif
