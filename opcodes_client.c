#include "p2pim.h"
#include "opcodes.h"

int handle_info(char *message, void* arg) {
    return 0;
}

void register_opcodes() {
    opcode_actions[SRV_INFO] = handle_info;
}
