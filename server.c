/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"
#include "structs.h"

int main(int argc, char **argv) {
    /* Linked list of clients. */
    Client *clients = NULL;
    
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
        //Client *client = find_client(clients, id);
/*        if (client != NULL) {
            printf("Client \"%s\" sent heartbeat\n", id);
            update_client(client);
        } else {
            printf("Client \"%s\" connected from %s:%d\n", id, address, port);
            add_client(&clients, id);
        }
        */
    }

    close(socket);

    return 0;
}
