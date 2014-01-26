/**
 * IM Client
 * Designed for secure peer-to-peer communication.
 */

#include "p2pim.h"
#include "net.h"
#include "structs_common.h"
#include "opcodes_client.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <hostname> <peer_id>\n", argv[0]);
        return 1;
    }

    register_opcodes();

    char *host = argv[1];
    char *peer_id = argv[2];

    struct peer server = { .id = "server" };

    int socket;
    struct packet_context p_ctx;
    struct opcode_context o_ctx = { .p_ctx = &p_ctx };

    socket = udp_connect(host, DEFAULT_SERVER_PORT, &server.sockaddr);

    prepare_packet(&p_ctx, CLIENT_REGISTER, peer_id);
    packet_dialog(socket, &server, &p_ctx);
    handle_opcode(&o_ctx);

    sleep(1);

    prepare_packet(&p_ctx, CLIENT_HEARTBEAT, "");
    packet_dialog(socket, &server, &p_ctx);
    handle_opcode(&o_ctx);

    sleep(1);

    prepare_packet(&p_ctx, CLIENT_LIST, "");
    packet_dialog(socket, &server, &p_ctx);
    handle_opcode(&o_ctx);

    close(socket);

    return 0;
}
