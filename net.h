#ifndef _NET_H
#define _NET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "p2pim.h"

#define MAX_PACKET_SIZE         768

/* Defined in structs_common.h */
struct peer;

/* Network utility functions */
void get_address(struct sockaddr *sa, char *address);
unsigned short get_port(struct sockaddr *sa);
int udp_bind(unsigned short port, struct addrinfo **conninfo);
int udp_connect(const char *host, unsigned short port, struct sockaddr *sockaddr);
int udp_send(int socket, struct sockaddr *dest_addr, void *data, size_t data_size);
int udp_recv(int socket, struct sockaddr *address, void *data);

#endif
