/**
 * IM Client
 * Designed for secure peer-to-peer communication.
 */

#include "p2pim.h"
#include "net.h"
#include "structs_common.h"
#include <ncurses.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <hostname> <peer_id>\n", argv[0]);
        return 1;
    }

    register_opcodes();

    /* Init NCURSES */
    initscr();
    keypad(stdscr, TRUE);
    printw("Hello!\n");
    refresh();

    char *host = argv[1];
    char *peer_id = argv[2];

    struct addrinfo *conninfo;
    struct sockaddr sockaddr;

    struct peer server = {
        .id = "server",
        .sockaddr = conninfo->ai_addr
    };

    int socket;
    int bytes;
    struct packet_context p_ctx;

    socket = udp_connect(host, SERVER_PORT, &conninfo);

    prepare_ctx(&p_ctx, CLI_REGISTER, peer_id);
    bytes = packet_send(socket, &server, &p_ctx);

    sleep(1);

    prepare_ctx(&p_ctx, CLI_HEARTBEAT, "");
    bytes = packet_send(socket, &server, &p_ctx);

    //udp_recv(socket, &srv, packet);
    //printw("Server response: %s\n",  packet);
    refresh();
/*    while (1) {
        scanf("%d%s", &opcode, message);
        bytes = udp_send(socket, conninfo, opcode, message);
    }*/

    close(socket);

    getch();
    endwin();

    return 0;
}
