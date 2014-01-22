#include "p2pim.h"
#include "structs_common.h"

/**
 * Create new peer structure
 */
struct peer* create_peer(char *id, char *address, unsigned short port, struct sockaddr *sockaddr) {
    struct peer *peer = malloc(sizeof(peer));

    /* Save peer's ID */
    strncpy(peer->id, id, MAX_PEER_ID_LEN);

    /* Save peer's location */
    strncpy(peer->public_addr.address, address, INET6_ADDRSTRLEN);
    peer->public_addr.port = port;

    /* Save connection info */
    peer->sockaddr = *sockaddr;

    return peer;
}

/**
 * Frees memory allocated by peer structure.
 */
void free_peer(struct peer *peer) {
    assert(peer != NULL);
    free(peer);
}
