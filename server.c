/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"
#include "opcodes.h"
#include "structs.h"

int main(int argc, char **argv) {
    register_opcodes();
    
    /* Bind socket */
    struct addrinfo *conninfo;
    int socket = udp_bind(SERVER_PORT, &conninfo);
    printf("Server listening on port %s...\n", SERVER_PORT);

    struct sockaddr_storage client_addr;
    char message[MAX_PACKET_SIZE];
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
    Opcode opcode;
    int numbytes;

    while (1) {
        numbytes = udp_recv(socket, (struct sockaddr *) &client_addr, &opcode, message);

        get_address((struct sockaddr *) &client_addr, address);
        port = get_port((struct sockaddr *) &client_addr);

        printf("Got opcode: %d with message: %s from %s:%d\n", opcode, message, address, port);

        Client *client = get_client(address, port);
        if (client != NULL) { 
            printf("Client found.\n");
            handle_opcode(opcode, message, client);
        } else {
            printf("Client not found.\n");
            Location location = { .port = port };
            strcpy(location.address, address);
            handle_opcode(opcode, message, &location);
        }
    }

    close(socket);

    return 0;
}
