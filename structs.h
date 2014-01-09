#pragma once

#include "common.h"

#define CLIENT_DURATION 30

typedef struct List {
    struct Client *client;
    struct Client *next;
} List;

int add_client(Client *client);
int delete_client(Client *client);
Client* get_client(char *address, unsigned short port);
Client* get_client_by_id(char *client_id);
void update_client(Client *client);

int send_opcode(int socket, Client *client, Opcode opcode, const char *message);
int send_opcode_status(int socket, Client *client, Opcode opcode, OpcodeStatus status);
