#pragma once

#include "common.h"

#define CLIENT_DURATION 30

typedef struct Node {
    Client *client;
    unsigned int time;
    struct Node *next;
} Node;

int add_node(Node **head, Client *client);
int delete_node(Node **head, Node *node);
Node* get_node(Node *head, char *address, unsigned short port);
Node* get_node_by_id(Node *head, char *client_id);
void free_node(Node *node);

int send_opcode(int socket, Client *client, Opcode opcode, const char *message);
int send_opcode_status(int socket, Client *client, Opcode opcode, OpcodeStatus status);
