/**
 * IM Client
 * Designed for secure peer-to-peer communication.
 */

#include "p2pim.h"
#include "opcodes.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <hostname> <client_id>\n", argv[0]);
        return 1;
    }

    register_opcodes();

    char *host = argv[1];
    char *client_id = argv[2];

    struct addrinfo *conninfo;
    int socket = udp_connect(host, SERVER_PORT, &conninfo);

    int bytes, opcode;
    char message[MAX_PACKET_SIZE];

    /* Send client ID to the server */
    bytes = udp_send(socket, conninfo, CLI_REGISTER, client_id);
    printf("sent %d bytes to %s\n", bytes, host);

    while (1) {
        printf("> ");
        scanf("%d%s", &opcode, message);
        bytes = udp_send(socket, conninfo, opcode, message);
        printf("sent %d bytes to %s\n", bytes, host);
    }

    close(socket);

    return 0;
}
