#pragma once

#include "structs.h"

#define OPCODES_COUNT   0x005

/*
 * CLI_ - opcodes sent by client
 * SRV_ - opcodes sent by server
 */
typedef enum Opcode {
    CLI_REGISTER     =   0x001,
    CLI_HEARTBEAT    =   0x002,
    CLI_CLOSE        =   0x003,
    CLI_QUERY        =   0x004,

    SRV_INFO         =   0x005
} Opcode;

void register_client_opcodes();
void register_server_opcodes();

int handle_opcode(Opcode opcode, Client *client, char *message);
