#pragma once

#define OPCODES_COUNT   0x006

/*
 * CLI_ - opcodes sent by client
 * SRV_ - opcodes sent by server
 */
typedef enum Opcode {
    CLI_REGISTER     =   0x000,
    CLI_HEARTBEAT    =   0x001,
    CLI_CLOSE        =   0x002,
    CLI_LIST         =   0x003,

    SRV_INFO         =   0x004
} Opcode;

typedef enum OpcodeStatus {
    STATUS_SUCCESS              =   0x000,
    ERROR_UNKNOWN               =   0x001,
    ERROR_NOT_REGISTERED        =   0x002,
    ERROR_CLIENT_ID_EXISTS      =   0x003
} OpcodeStatus;

struct OpcodeData;

typedef struct OpcodeData OpcodeData;
extern char* StatusMessages[];

/* Opcodes array */
extern int (*opcode_actions[OPCODES_COUNT]) (OpcodeData *data);
extern int handle_opcode(OpcodeData *data);

void register_opcodes();
