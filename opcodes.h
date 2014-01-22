#ifndef _OPCODES_H
#define _OPCODES_H

#define OPCODES_COUNT   0x006

/*
 * CLI_ - opcodes sent by client
 * SRV_ - opcodes sent by server
 */
enum opcode {
    CLI_REGISTER     =   0x000,
    CLI_HEARTBEAT    =   0x001,
    CLI_CLOSE        =   0x002,
    CLI_LIST         =   0x003,

    SRV_INFO         =   0x004
};

enum opcode_status {
    STATUS_SUCCESS              =   0x000,
    ERROR_UNKNOWN               =   0x001,
    ERROR_NOT_REGISTERED        =   0x002,
    ERROR_PEER_ID_EXISTS        =   0x003
};

struct opcode_context;

extern char* status_messages[];

/* Opcodes array */
extern int (*opcode_actions[OPCODES_COUNT]) (struct opcode_context *ctx);
extern int handle_opcode(struct opcode_context *ctx);

void register_opcodes();

#endif
