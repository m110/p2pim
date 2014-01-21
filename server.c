/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"
#include "net.h"
#include "opcodes_server.h"
#include "structs_server.h"

int main(int argc, char **argv) {
    register_opcodes();

    /* Linked list of clients. */
    struct node *clients = NULL;

    /* Bind socket */
    struct addrinfo *conninfo;
    int socket = udp_bind(SERVER_PORT, &conninfo);
    printf("Server listening on port %s...\n", SERVER_PORT);

    struct sockaddr_storage *client_info;
    char packet[MAX_PACKET_SIZE];
    char message[MAX_PACKET_SIZE];
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
    enum opcode opcode;
    int numbytes;

    for (;;) {
        memset(packet, 0, sizeof(packet));
        client_info = malloc(sizeof(struct sockaddr_storage));
        numbytes = udp_recv(socket, (struct sockaddr *) client_info, packet);
        unpack_packet(packet, &opcode, message);

        get_address((struct sockaddr *) client_info, address);
        port = get_port((struct sockaddr *) client_info);

        printf("Got opcode: %d with message: %s from %s:%d\n", opcode, message, address, port);

        struct opcode_context ctx = { .opcode = opcode, .message = message, .list = clients };

        struct node *node = get_node(clients, address, port);
        if (node != NULL) {
            printf("Client found.\n");
            free(client_info);
            ctx.node = node;
            handle_opcode(&ctx);
        } else {
            printf("New client\n");
            struct client *client = create_client(message, address, port,
                    (struct sockaddr *) client_info);

            ctx.client = client;
            int error = handle_opcode(&ctx);
            if (error) {
                printf("handle_opcode error: %s\n", status_messages[error]);
                //pack_packet(packet, SRV_INFO, itoa(error));
                //udp_send(socket, client->addr, packet);
                free_client(client);
            }
        }
    }

    close(socket);

    return 0;
}
