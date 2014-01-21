#ifndef _STRUCTS_COMMON_H
#define _STRUCTS_COMMON_H

#include "net.h"

typedef struct Location {
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
} Location;

typedef struct Client {
    char *id;
    Location public_addr;
    struct sockaddr *sockaddr;
} Client;

Client* create_client(char *id, char *address, unsigned short port, struct sockaddr *sockaddr);
void free_client(Client *client);

#endif
