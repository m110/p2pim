/**
 * IM Server
 * Provides UDP hole punching.
 */

#include <signal.h>
#include "p2pim.h"
#include "net.h"
#include "opcodes_server.h"
#include "structs_server.h"

void sigint_callback(int signum) {
    printf("Good bye.");
    exit(signum);
}

int main(int argc, char **argv) {
    unsigned short port = DEFAULT_SERVER_PORT;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    signal(SIGINT, sigint_callback);
    register_opcodes();

    /* Linked list of clients. */
    struct node *clients = NULL;

    /* Bind socket */
    struct addrinfo *conninfo;
    int socket = udp_bind(port, &conninfo);
    printf("Server listening on port %d...\n", port);

    int status;
    struct peer client;
    struct packet_context p_ctx;
    struct opcode_context o_ctx;

    for (;;) {
        packet_recv(socket, &client, &p_ctx);

        o_ctx.p_ctx = &p_ctx;
        o_ctx.list = &clients;
        o_ctx.peer = &client;
        o_ctx.node = get_node(clients, &client.public_addr);

        printf("Peer from %s:%d (%s) ", client.public_addr.address,
                client.public_addr.port,
                o_ctx.node == NULL ? "not found" : "registered");
        print_packet("sent packet: ", &p_ctx);

        status = handle_opcode(&o_ctx);

        prepare_status(&p_ctx, status);
        packet_send(socket, &client, &p_ctx);

        if (status) {
            printf("handle_opcode error: %s\n", status_messages[status]);
        }
    }

    close(socket);

    return 0;
}
