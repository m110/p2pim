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

    int bytes;
    struct peer *client;
    struct packet_context p_ctx;

    for (;;) {
        packet_recv(socket, &client, &p_ctx);

        printf("Got opcode: %d with message: %s from %s:%d\n", 
                p_ctx.opcode, p_ctx.message, client->public_addr.address,
                client->public_addr.port);

        struct opcode_context o_ctx = { .p_ctx = &p_ctx, .list = clients };
        struct node *node = get_node(clients, &client->public_addr);

        if (node != NULL) {
            printf("Client found.\n");
            free_peer(client);
            o_ctx.node = node;
            handle_opcode(&o_ctx);
        } else {
            printf("New client\n");

            o_ctx.peer = client;
            int error = handle_opcode(&o_ctx);
            if (error) {
                printf("handle_opcode error: %s\n", status_messages[error]);
                //pack_packet(packet, SRV_INFO, itoa(error));
                //udp_send(socket, client->addr, packet);
                free_peer(client);
            }
        }
    }

    close(socket);

    return 0;
}
