/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"
#include "opcodes.h"
#include "structs.h"

int main(int argc, char **argv) {
    register_opcodes();

    /* Linked list of clients. */
    Node *clients = NULL;

    /* Bind socket */
    struct addrinfo *conninfo;
    int socket = udp_bind(SERVER_PORT, &conninfo);
    printf("Server listening on port %s...\n", SERVER_PORT);

    struct sockaddr_storage *client_info;
    char packet[MAX_PACKET_SIZE];
    char message[MAX_PACKET_SIZE];
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
    Opcode opcode;
    int numbytes;

    for (;;) {
        memset(packet, 0, sizeof(packet));
        client_info = malloc(sizeof(struct sockaddr_storage));
        numbytes = udp_recv(socket, (struct sockaddr *) client_info, packet);
        unpack_packet(packet, &opcode, message);

        get_address((struct sockaddr *) client_info, address);
        port = get_port((struct sockaddr *) client_info);

        printf("Got opcode: %d with message: %s from %s:%d\n", opcode, message, address, port);

        Node *node = get_node(clients, address, port);
        if (node != NULL) {
            printf("Client found.\n");
            free(client_info);
            handle_opcode(opcode, message, node);
        } else {
            Client *client = create_client(message, address, port,
                    (struct sockaddr *) client_info);

            int error = handle_opcode(opcode, message, node);
            if (error) {
                printf("handle_opcode error: %s\n", StatusMessages[error]);
                //pack_packet(packet, SRV_INFO, itoa(error));
                //udp_send(socket, client->addr, packet);
                delete_node(&clients, node);
            }
        }
    }

    close(socket);

    return 0;
}
