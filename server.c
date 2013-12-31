/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"

#define MAXBUFLEN 100

/* Returns in_addr for IPv4 or IPv6 */
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    } else {
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }
}

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
     * Waiting for connections
     */

    printf("Server ready, waiting for connections...\n");

    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof client_addr;
    char buffer[MAXBUFLEN];
    char address[INET6_ADDRSTRLEN];
    int numbytes;

    if ((numbytes = recvfrom(sockfd, buffer, MAXBUFLEN-1 , 0,
        (struct sockaddr *)&client_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(102);
    }

    printf("Received from %s\n",
        inet_ntop(client_addr.ss_family,
            get_in_addr((struct sockaddr *)&client_addr),
            address, sizeof address));
    printf("packet is %d bytes long\n", numbytes);
    buffer[numbytes] = '\0';
    printf("packet contains: \"%s\"\n", buffer);

    close(sockfd);

    return 0;
}
