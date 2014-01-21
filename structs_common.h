#ifndef _STRUCTS_COMMON_H
#define _STRUCTS_COMMON_H

#include "net.h"

struct net_location {
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
};

struct client {
    char *id;
    struct net_location public_addr;
    struct sockaddr *sockaddr;
};

struct client* create_client(char *id, char *address, unsigned short port, struct sockaddr *sockaddr);
void free_client(struct client *client);

#endif
