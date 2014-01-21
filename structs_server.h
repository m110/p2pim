#ifndef _STRUCTS_SERVER_H
#define _STRUCTS_SERVER_H

#include "structs_common.h"

#define CLIENT_DURATION 30

struct node {
    struct client *client;
    unsigned int time;
    struct node *next;
};

int add_node(struct node **head, struct client *client);
int delete_node(struct node **head, struct node *node);
struct node* get_node(struct node *head, char *address, unsigned short port);
struct node* get_node_by_id(struct node *head, char *client_id);
void free_node(struct node *node);

int send_opcode(int socket, struct client *client, enum opcode opcode, const char *message);
int send_opcode_status(int socket, struct client *client, enum opcode opcode, enum opcode_status status);

#endif
