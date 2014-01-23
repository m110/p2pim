/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"
#include "net.h"
#include "opcodes_server.h"
#include "structs_server.h"

int main(int argc, char **argv) {
    unsigned short port = DEFAULT_SERVER_PORT;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    register_opcodes();

    /* Linked list of clients. */
    struct node *clients = NULL;

    /* Bind socket */
    struct addrinfo *conninfo;
    int socket = udp_bind(port, &conninfo);
    printf("Server listening on port %d...\n", port);

    struct peer client;
    struct packet_context p_ctx;

    for (;;) {
        packet_recv(socket, &client, &p_ctx);

        printf("Got opcode: %d with message: %s from %s:%d\n", 
                p_ctx.opcode, p_ctx.message, client.public_addr.address,
                client.public_addr.port);

        struct opcode_context o_ctx = { .p_ctx = &p_ctx, .list = clients };
        struct node *node = get_node(clients, &client.public_addr);

        if (node != NULL) {
            printf("Client found.\n");
            o_ctx.node = node;
            handle_opcode(&o_ctx);
        } else {
            printf("New client\n");

            o_ctx.peer = &client;
            int error = handle_opcode(&o_ctx);
            if (error) {
                printf("handle_opcode error: %s\n", status_messages[error]);
                // TODO send error message
            }
        }
    }

    close(socket);

    return 0;
}
