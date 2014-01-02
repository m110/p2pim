#pragma once

typedef enum Opcode {
    OC_REGISTER     =   0x001,
    OC_HEARTBEAT    =   0x002,
    OC_CLOSE        =   0x003,
    OC_QUERY        =   0x004
} Opcode;
