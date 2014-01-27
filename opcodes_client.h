#ifndef _OPCODES_CLIENT_H
#define _OPCODES_CLIENT_H

#include "structs_common.h"
#include "opcodes.h"
#include "packet.h"
#include "include/tpl.h"

struct opcode_context  {
    struct packet_context *p_ctx;
};

#endif
