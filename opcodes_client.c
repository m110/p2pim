#include "p2pim.h"
#include "opcodes.h"

struct OpcodeData {
    Opcode opcode;
    char *message;
};

int handle_info(OpcodeData data) {
    return 0;
}

void register_opcodes() {
    opcode_actions[SRV_INFO] = handle_info;
}
