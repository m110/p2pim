#include "p2p.h"
#include "common.h"

/* Create new client structure */
Client* create_client(char *id, char *address, unsigned short port, struct sockaddr *sockaddr) {
    Client *client = malloc(sizeof(Client));
    client->id = strdup(id);
    client->time = current_time();

    /* Save client's location */
    strcpy(client->public_addr.address, address);
    client->public_addr.port = port;

    /* Save connection info */
    client->sockaddr = sockaddr;

    return client;
}

