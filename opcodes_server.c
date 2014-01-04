#include "p2pim.h"
#include "opcodes.h"
#include "structs.h"

int handle_register(char *message, void *arg) {
    Client *client = (Client*) arg;
    int error = add_client(client);

    if (error) {
        delete_client(client);
        return error;
    }

    return 0;
}

int handle_heartbeat(char *message, void* arg) {
    Client *client = (Client*) arg;
    assert(client != NULL);

    update_client(client);
    return 0;
}

int handle_close(char *message, void* arg) {
    Client *client = (Client*) arg;
    assert(client != NULL);

    delete_client(client);
    return 0;
}

int handle_list(char *message, void* arg) {
    return 0;
}

int handle_query(char *message, void* arg) {
    return 0;
}

void register_opcodes() {
    opcode_actions[CLI_REGISTER] = handle_register;
    opcode_actions[CLI_HEARTBEAT] = handle_heartbeat;
    opcode_actions[CLI_CLOSE] = handle_close;
    opcode_actions[CLI_LIST] = handle_list;
    opcode_actions[CLI_QUERY] = handle_query;
}
