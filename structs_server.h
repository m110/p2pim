#ifndef _STRUCTS_SERVER_H
#define _STRUCTS_SERVER_H

#include "structs_common.h"

#define peer_DURATION 30

struct node {
    struct peer peer;
    unsigned int time;
    struct node *next;
};

int add_node(struct node **head, struct peer *peer);
int delete_node(struct node **head, struct node *node);
struct node* get_node(struct node *head, struct net_location *location);
struct node* get_node_by_id(struct node *head, char *peer_id);
void free_node(struct node *node);

int send_opcode(int socket, struct peer *peer, enum opcode opcode, const char *message);
int send_opcode_status(int socket, struct peer *peer, enum opcode opcode, enum opcode_status status);

#endif
