#include "p2pim.h"
#include "structs_common.h"

/**
 * Create new client structure
 */
struct client* create_client(char *id, char *address, unsigned short port, struct sockaddr *sockaddr) {
    struct client *client = malloc(sizeof(client));
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
void free_client(struct client *client) {
    assert(client != NULL);

    free(client->id);
    free(client->sockaddr);
    free(client);
}
