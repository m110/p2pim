/**
 * IM Client
 * Designed for secure peer-to-peer communication.
 */

#include "p2pim.h"
#include "opcodes.h"
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
    printw("Hello!");
    refresh();

    char *host = argv[1];
    char *client_id = argv[2];

    struct addrinfo *conninfo;
    int socket = udp_connect(host, SERVER_PORT, &conninfo);

    int bytes, opcode;
    char packet[MAX_PACKET_SIZE];
    pack_packet(packet, CLI_REGISTER, client_id);

    /* Send client ID to the server */
    bytes = udp_send(socket, conninfo, packet);

/*    while (1) {
        scanf("%d%s", &opcode, message);
        bytes = udp_send(socket, conninfo, opcode, message);
    }*/

    close(socket);

    getch();
    endwin();

    return 0;
}
