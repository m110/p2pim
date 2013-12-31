/**
 * IM Server
 * Provides UDP hole punching.
 */

#include "p2pim.h"
#include "structs.h"

#define MAXBUFLEN 100

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void* cleanup_clients(void *_list) {
    Client **list = (Client **) _list;

    const unsigned int interval = 1;
    const unsigned int duration = 5;
    unsigned int time = current_time();

    while (1) {
        if (current_time() - time < interval) {
            continue;
        }

        Client *prev = *list;
        for (Client *c = *list; c != NULL; c = c->next) {
            printf("Hello, : %s\n", c->id);
            if (current_time() - c->time > duration) {
                pthread_mutex_lock(&clients_mutex);
                delete_client(list, c);
                pthread_mutex_unlock(&clients_mutex);

                if (prev != NULL) {
                    c = prev;
                }
            }
            prev = c;
        }

        time = current_time();
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
     * Connection ready.
     */

    /* Linked list of clients. */
    Client *clients = NULL;

    pthread_t cleanup_thread;
    pthread_create(&cleanup_thread, NULL, cleanup_clients, (void*) &clients);

    printf("Server ready, waiting for connections...\n");

    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof client_addr;
    char buffer[MAXBUFLEN];
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
    int numbytes;

    while (1) {
        if ((numbytes = recvfrom(sockfd, buffer, MAXBUFLEN-1 , 0,
            (struct sockaddr *)&client_addr, &addr_len)) == -1) {
            perror("recvfrom");
            exit(102);
        }

        get_address((struct sockaddr *) &client_addr, address);
        port = get_port((struct sockaddr *) &client_addr);
        printf("Received from %s port: %d\n", address, port);
        printf("packet is %d bytes long\n", numbytes);
        buffer[numbytes] = '\0';
        printf("packet contains: \"%s\"\n", buffer);

        add_client(&clients, buffer);
    }

    pthread_join(cleanup_thread, NULL);

    close(sockfd);

    return 0;
}
