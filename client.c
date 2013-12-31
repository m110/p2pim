/**
 * IM Client
 * Designed for secure peer-to-peer communication.
 */

#include "p2pim.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <hostname> <client_id>\n", argv[0]);
        return 1;
    }

    char *host = argv[1];
    char *client_id = argv[2];

    int sockfd;
    int rv;
    struct addrinfo *servinfo, *p;

    /* Initialize hints structure */
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(host, SERVER_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(100);
    }

    /* Loop through results and create a socket */
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to bind a socket!\n");
        exit(101);
    }

    /* Send client ID to the server */
    int numbytes;
    if ((numbytes = sendto(sockfd, client_id, strlen(client_id), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("sendto");
        exit(102);
    }

    freeaddrinfo(servinfo);

    printf("sent %d bytes to %s\n", numbytes, host);
    close(sockfd);

    return 0;
}
