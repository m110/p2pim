#include "p2pim.h"
#include "structs_server.h"

/**
 * Appends existing peer structure to the peers list.
 */
int add_node(struct node **head, struct peer *peer) {
    struct node *last = *head;
    for (; last != NULL && last->next != NULL; last = last->next);

    /* Create new node for struct peer */
    struct node *node = malloc(sizeof(node));
    node->peer = peer;
    node->time = current_time();
    node->next = NULL;

    /* If the list is empty, set the new node as head,
     * otherwise append it at the end
     */
    if (last == NULL) {
        *head = node;
    } else {
        last->next = node;
    }

    return 0;
}

/**
 * Deletes peer from list.
 * The node structure is then freed by free_node.
 */
int delete_node(struct node **head, struct node *node) {
    /* First case: node is the head of list */
    if (node == *head) {
        *head = (*head)->next;
        free_node(node);
        return 0;
    }

    /* Second case: search the list for struct node */
    for (struct node *node = *head; node != NULL; node = node->next) {
        /* peer found in the next node  */
        if (node->next != NULL && node->next == node) {
            node->next = node->next->next;
            free_node(node);
            return 0;
        }
    }

    return 1;
}

/**
 * Returns node found in list by peer address and port.
 * Returns NULL if node was not found.
 */
struct node* get_node(struct node *head, struct net_location *location) {
    for (struct node *node = head; node != NULL; node = node->next) {
        if (strcmp(node->peer->public_addr.address, location->address) == 0 &&
            node->peer->public_addr.port == location->port) {
            return node;
        }
    }

    return NULL;
}

/**
 * Returns node found in list by peer id.
 * Returns NULL if node was not found.
 */
struct node* get_node_by_id(struct node *head, char *peer_id) {
    for (struct node *node = head; node != NULL; node = node->next) {
        if (strcmp(node->peer->id, peer_id) == 0) {
            return node;
        }
    }

    return NULL;
}

/**
 * Frees memory allocated by node structure. Calls free_peer.
 */
void free_node(struct node *node) {
    assert(node != NULL);

    free_peer(node->peer);
    free(node);
}
