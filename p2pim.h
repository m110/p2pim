#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <assert.h>

#include "opcodes.h"

#define MAX_PACKET_SIZE     256
#define SERVER_PORT         "5000"

/* General utility functions */
unsigned int current_time();
char* strdup(const char *s);

/* Network utility functions */
void get_address(struct sockaddr *sa, char *address);
unsigned short get_port(struct sockaddr *sa);
int udp_bind(const char *port, struct addrinfo **conninfo);
int udp_connect(const char *host, const char *port, struct addrinfo **conninfo);
int udp_send(int socket, struct addrinfo *conninfo, Opcode opcode, 
        const char *message);
int udp_recv(int socket, struct sockaddr *conninfo, Opcode *opcode,
        char *message);
