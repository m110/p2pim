/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"
#include "structs.h"

#define MAXBUFLEN 100

int main(int argc, char **argv) {
    int sockfd;
    int rv;
    struct addrinfo *servinfo, *p;

    /* Initialize hints structure */
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, SERVER_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(100);
    }

    /* Loop through results and bind if possible */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to bind a socket!\n");
        exit(101);
    }

    freeaddrinfo(servinfo);

    /*
     * Connection ready.
     */

    /* Linked list of clients. */
    Client *clients = NULL;

    printf("Server ready, waiting for connections...\n");

    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof client_addr;
    char id[MAXBUFLEN];
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
    int numbytes;

    while (1) {
        if ((numbytes = recvfrom(sockfd, id, MAXBUFLEN-1 , 0,
            (struct sockaddr *)&client_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(102);
        }

        get_address((struct sockaddr *) &client_addr, address);
        port = get_port((struct sockaddr *) &client_addr);
        id[numbytes] = '\0';

        Client *client = find_client(clients, id);
        if (client != NULL) {
            printf("Client \"%s\" sent heartbeat\n", id);
            update_client(client);
        } else {
            printf("Client \"%s\" connected from %s:%d\n", id, address, port);
            add_client(&clients, id);
        }
    }

    close(sockfd);

    return 0;
}
