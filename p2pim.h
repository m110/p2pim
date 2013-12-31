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

#define SERVER_PORT "5000"

unsigned int current_time();
void get_address(struct sockaddr *sa, char *address);
unsigned short get_port(struct sockaddr *sa);

char* strdup(const char *s);
