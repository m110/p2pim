#ifndef _NET_H
#define _NET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "structs_common.h"
#include "opcodes.h"

#define MAX_PACKET_SIZE         532
#define MAX_MESSAGE_LEN         512

/* Defined in structs_common.h */
struct peer;

struct packet_context {
    enum opcode opcode;
    char message[MAX_MESSAGE_LEN];
};

int prepare_ctx(struct packet_context *p_ctx, enum opcode opcode, char *message);

/* Network utility functions */
void get_address(struct sockaddr *sa, char *address);
unsigned short get_port(struct sockaddr *sa);
int udp_bind(const char *port, struct addrinfo **conninfo);
int udp_connect(const char *host, const char *port, struct addrinfo **conninfo);
int udp_send(int socket, struct sockaddr *dest_addr, const char *packet);
int udp_recv(int socket, struct sockaddr *address, char *packet);
int packet_send(int socket, struct peer *peer, struct packet_context *p_ctx);
int packet_recv(int socket, struct peer **peer, struct packet_context *p_ctx);

#endif
