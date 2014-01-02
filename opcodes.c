#include "p2pim.h"
#include "opcodes.h"

int (*opcode_actions[OPCODES_COUNT]) (Client *client, char *message);

int handle_register(Client *client, char *message) {
    return 0;
}

int handle_heartbeat(Client *client, char *message) {
    return 0;
}

int handle_close(Client *client, char *message) {
    return 0;
}

int handle_query(Client *client, char *message) {
    return 0;
}

int handle_info(Client *client, char *message) {
    return 0;
}

void register_client_opcodes() {
    opcode_actions[CLI_REGISTER] = handle_register;
    opcode_actions[CLI_HEARTBEAT] = handle_heartbeat;
    opcode_actions[CLI_CLOSE] = handle_close;
    opcode_actions[CLI_QUERY] = handle_query;
}

void register_server_opcodes() {
    opcode_actions[SRV_INFO] = handle_info;
}

int handle_opcode(Opcode opcode, Client *client, char *message) {
    (*opcode_actions[opcode])(client, message);
}
