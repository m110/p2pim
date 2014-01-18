#include "p2p.h"
#include "common.h"

/**
 * Create new client structure
 */
Client* create_client(char *id, char *address, unsigned short port, struct sockaddr *sockaddr) {
    Client *client = malloc(sizeof(Client));
    client->id = strdup(id);

    /* Save client's location */
    strcpy(client->public_addr.address, address);
    client->public_addr.port = port;

    /* Save connection info */
    client->sockaddr = sockaddr;

    return client;
}

/**
 * Frees memory allocated by client structure.
 */
void free_client(Client *client) {
    assert(client == NULL);

    free(client->id);
    free(client->sockaddr);
    free(client);
}
