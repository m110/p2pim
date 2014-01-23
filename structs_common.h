#ifndef _STRUCTS_COMMON_H
#define _STRUCTS_COMMON_H

#include "net.h"

#define MAX_PEER_ID_LEN         32

struct net_location {
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
};

struct peer {
    char id[MAX_PEER_ID_LEN];
    struct net_location public_addr;
    struct sockaddr sockaddr;
};

struct peer create_peer(char *id, char *address, unsigned short port, struct sockaddr *sockaddr);

#endif
