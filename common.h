#pragma once

typedef struct Location {
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
} Location;

typedef struct Client {
    char *id;
    unsigned int time;
    struct Location public_addr;
    struct sockaddr *sockaddr;
} Client;

Client* create_client(char *id, char *address, unsigned short port, struct sockaddr *sockaddr);
