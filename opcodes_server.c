#include "p2pim.h"
#include "opcodes.h"
#include "opcodes_server.h"

int handle_register(OpcodeData *data) {
    Client *client = data->client;
    assert(client != NULL);

    if (get_node_by_id(data->list, client->id) == NULL) {
        printf("Register user %s from %s:%d\n",
                client->id, 
                client->public_addr.address, 
                client->public_addr.port);

        return add_node(&data->list, client);
    } else {
        return ERROR_CLIENT_ID_EXISTS;
    }

    return STATUS_SUCCESS;
}

int handle_heartbeat(OpcodeData *data) {
    Node *node = data->node;
    if (node == NULL) {
        return ERROR_NOT_REGISTERED;
    }

    node->time = current_time();
    return 0;
}

int handle_close(OpcodeData *data) {
    Node *node = data->node;
    if (node == NULL) {
        return ERROR_NOT_REGISTERED;
    }

    delete_node(&data->list, node);
    return 0;
}

int handle_list(OpcodeData *data) {
    Node *node = data->node;
    if (node == NULL) {
        return ERROR_NOT_REGISTERED;
    }

    return 0;
}

void register_opcodes() {
    opcode_actions[CLI_REGISTER] = handle_register;
    opcode_actions[CLI_HEARTBEAT] = handle_heartbeat;
    opcode_actions[CLI_CLOSE] = handle_close;
    opcode_actions[CLI_LIST] = handle_list;
}

int send_opcode(int socket, Client *client, Opcode opcode, const char *message) {
    char packet[MAX_PACKET_SIZE];
    pack_packet(packet, opcode, message);
    return udp_send(socket, client->sockaddr, packet);
}

int send_opcode_status(int socket, Client *client, Opcode opcode, OpcodeStatus status) {

}

