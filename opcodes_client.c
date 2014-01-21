#include "p2pim.h"
#include "opcodes.h"
#include "opcodes_client.h"

int handle_info(OpcodeData *data) {
    return 0;
}

void register_opcodes() {
    opcode_actions[SRV_INFO] = handle_info;
}
