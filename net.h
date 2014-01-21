#ifndef _NET_H
#define _NET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "opcodes.h"

#define MAX_PACKET_SIZE     256

/* Network utility functions */
void get_address(struct sockaddr *sa, char *address);
unsigned short get_port(struct sockaddr *sa);
int udp_bind(const char *port, struct addrinfo **conninfo);
int udp_connect(const char *host, const char *port, struct addrinfo **conninfo);
int udp_send(int socket, struct sockaddr *dest_addr, const char *packet);
int udp_recv(int socket, struct sockaddr *address, char *packet);
void pack_packet(char *packet, enum opcode opcode, const char *message);
void unpack_packet(char *packet, enum opcode *opcode, char *message);

#endif
