#include "p2pim.h"
#include "structs_common.h"
#include "net.h"
#include "tpl/tpl.h"

int prepare_ctx(struct packet_context *p_ctx, enum opcode opcode, char *message) {
    p_ctx->opcode = opcode;
    p_ctx->message = strdup(message);
    return 0;
}

/* Get IP address from sockadd struct */
void get_address(struct sockaddr *sa, char *address) {
    if (sa->sa_family == AF_INET) {
        inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                address, INET_ADDRSTRLEN);
    } else {
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                address, INET6_ADDRSTRLEN);
    }
}

/* Get port number from sockaddr struct */
unsigned short get_port(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return ((struct sockaddr_in *)sa)->sin_port;
    } else {
        return ((struct sockaddr_in6 *)sa)->sin6_port;
    }
}

/* Bind UDP socket on local host */
int udp_bind(unsigned short port, struct addrinfo **conninfo) {
    int sockfd, rv;
    struct addrinfo *servinfo;
    char service[6];

    /* Initialize hints structure */
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    /* Set service (port) */
    sprintf(service, "%u", port);

    if ((rv = getaddrinfo(NULL, service, &hints, &servinfo)) != 0) {
        fprintf(stderr, "udp_bind getaddrinfo: %s\n", gai_strerror(rv));
        exit(100);
    }

    /* Loop through results and bind if possible */
    struct addrinfo *p;
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("udp_bind socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("udp_bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "udp_bind: Failed to bind a socket!\n");
        exit(101);
    }

    freeaddrinfo(servinfo);

    *conninfo = p;
    return sockfd;
}

/* Get given UDP socket */
int udp_connect(const char *host, unsigned short port, struct addrinfo **conninfo) {
    int sockfd, rv;
    struct addrinfo *servinfo;
    char service[6];

    /* Initialize hints structure */
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    /* Set service (port) */
    sprintf(service, "%u", port);

    if ((rv = getaddrinfo(host, service, &hints, &servinfo)) != 0) {
        fprintf(stderr, "udp_connect getaddrinfo: %s\n", gai_strerror(rv));
        exit(200);
    }

    /* Loop through results and create a socket */
    struct addrinfo *p;
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("udp_connect socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "udp_connect: Failed to bind a socket!\n");
        exit(201);
    }

    freeaddrinfo(servinfo);

    *conninfo = p;
    return sockfd;
}

int udp_send(int socket, struct sockaddr *sockaddr, void *data) {
    int bytes;
    socklen_t addr_len = sizeof *sockaddr;

    if ((bytes = sendto(socket, data, MAX_PACKET_SIZE, 0,
             sockaddr, addr_len)) == -1) {
        perror("udp_send sendto");
        exit(300);
    }

    return bytes;
}

int udp_recv(int socket, struct sockaddr *sockaddr, void *data) {
    int bytes;
    socklen_t addr_len = sizeof *sockaddr;

    if ((bytes = recvfrom(socket, data, MAX_PACKET_SIZE-1, 0,
            sockaddr, &addr_len)) == -1) {
        perror("udp_recv recvfrom");
        exit(400);
    }

    return bytes;
}

/* Send packet to peer */
int packet_send(int socket, struct peer *peer, struct packet_context *p_ctx) {
    void *data;
    size_t data_size;
    tpl_node *packet;

    packet = tpl_map("S(is)", p_ctx);
    tpl_pack(packet, 0);
    tpl_dump(packet, TPL_MEM, &data, &data_size);
    tpl_free(packet);

    free(p_ctx->message);

    return udp_send(socket, &peer->sockaddr, data);
}

/* Receive packet from peer */
int packet_recv(int socket, struct peer *peer, struct packet_context *p_ctx) {
    char *data;
    tpl_node *packet;
    struct sockaddr_storage sockaddr;
    int bytes;

    char address[INET6_ADDRSTRLEN];
    unsigned short port;

    bytes = udp_recv(socket, (struct sockaddr *) &sockaddr, &data);
    get_address((struct sockaddr *) &sockaddr, address);
    port = get_port((struct sockaddr *) &sockaddr);

    packet = tpl_map("S(is)", p_ctx);
    tpl_load(packet, TPL_MEM|TPL_EXCESS_OK, data, bytes);
    tpl_unpack(packet, 0);
    tpl_free(packet);

    *peer = create_peer("temporary", address, port, (struct sockaddr *) &sockaddr);

    return bytes;
}
