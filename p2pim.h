#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_PORT "5000"

struct Location {
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
};

struct Client {
    struct Location private_addr;
    struct Location public_addr;
};

void get_address(struct sockaddr *sa, char *address);
unsigned short get_port(struct sockaddr *sa);
