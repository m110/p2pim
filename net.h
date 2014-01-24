#ifndef _NET_H
#define _NET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "structs_common.h"
#include "opcodes.h"

#define MAX_PACKET_SIZE         1024
#define MAX_MESSAGE_LEN         512

#define PACKET_TPL_FORMAT       "S(iis)"

/* Defined in structs_common.h */
struct peer;

struct packet_context {
    enum opcode opcode;
    enum opcode_status status;
    char *message;
};

int prepare_packet(struct packet_context *p_ctx, enum opcode opcode, char *message);
int prepare_status(struct packet_context *p_ctx, enum opcode_status status);
void print_packet(char *prefix, struct packet_context *p_ctx);

/* Network utility functions */
void get_address(struct sockaddr *sa, char *address);
unsigned short get_port(struct sockaddr *sa);
int udp_bind(unsigned short port, struct addrinfo **conninfo);
int udp_connect(const char *host, unsigned short port, struct sockaddr *sockaddr);
int udp_send(int socket, struct sockaddr *dest_addr, void *data, size_t data_size);
int udp_recv(int socket, struct sockaddr *address, void *data);
int packet_send(int socket, struct peer *peer, struct packet_context *p_ctx);
int packet_recv(int socket, struct peer *peer, struct packet_context *p_ctx);
int packet_dialog(int socket, struct peer *peer, struct packet_context *p_ctx);

#endif
