#pragma once

#define OPCODES_COUNT   0x006

/*
 * CLI_ - opcodes sent by client
 * SRV_ - opcodes sent by server
 */
typedef enum Opcode {
    CLI_REGISTER     =   0x001,
    CLI_HEARTBEAT    =   0x002,
    CLI_CLOSE        =   0x003,
    CLI_LIST         =   0x004,
    CLI_QUERY        =   0x005,

    SRV_INFO         =   0x005
} Opcode;

/* Opcodes array */
int (*opcode_actions[OPCODES_COUNT]) (char *message, void* arg);

static inline int handle_opcode(Opcode opcode, char *message, void* arg) {
    return (*opcode_actions[opcode])(message, arg);
}

void register_opcodes();
