/**
 * IM Client
 * Designed for secure peer-to-peer communication.
 */

#include "p2pim.h"
#include "net.h"
#include <ncurses.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <hostname> <client_id>\n", argv[0]);
        return 1;
    }

    register_opcodes();

    /* Init NCURSES */
    initscr();
    keypad(stdscr, TRUE);
    printw("Hello!\n");
    refresh();

    char *host = argv[1];
    char *client_id = argv[2];

    struct addrinfo *conninfo;
    int socket = udp_connect(host, SERVER_PORT, &conninfo);

    int bytes, opcode;
    char packet[MAX_PACKET_SIZE];

    /* Send client ID to the server */
    pack_packet(packet, CLI_REGISTER, client_id);
    bytes = udp_send(socket, conninfo->ai_addr, packet);

    sleep(1);

    pack_packet(packet, CLI_HEARTBEAT, "");
    bytes = udp_send(socket, conninfo->ai_addr, packet);

    struct sockaddr srv;
    udp_recv(socket, &srv, packet);
    printw("Server response: %s\n",  packet);
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
