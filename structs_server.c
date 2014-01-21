#include "p2pim.h"
#include "structs_server.h"

/**
 * Appends existing client structure to the clients list.
 */
int add_node(Node **head, Client *client) {
    Node *last = *head;
    for (; last != NULL && last->next != NULL; last = last->next);

    /* Create new node for client */
    Node *node = malloc(sizeof(Node));
    node->client = client;
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
 * Deletes client from list.
 * The node structure is then freed by free_node.
 */
int delete_node(Node **head, Node *node) {
    /* First case: node is the head of list */
    if (node == *head) {
        *head = (*head)->next;
        free_node(node);
        return 0;
    }

    /* Second case: search the list for node */
    for (Node *node = *head; node != NULL; node = node->next) {
        /* Client found in the next node  */
        if (node->next != NULL && node->next == node) {
            node->next = node->next->next;
            free_node(node);
            return 0;
        }
    }

    return 1;
}

/**
 * Returns node found in list by client address and port.
 * Returns NULL if node was not found.
 */
Node* get_node(Node *head, char *address, unsigned short port) {
    for (Node *node = head; node != NULL; node = node->next) {
        if (strcmp(node->client->public_addr.address, address) == 0 &&
            node->client->public_addr.port == port) {
            return node;
        }
    }

    return NULL;
}

/**
 * Returns node found in list by client id.
 * Returns NULL if node was not found.
 */
Node* get_node_by_id(Node *head, char *client_id) {
    for (Node *node = head; node != NULL; node = node->next) {
        if (strcmp(node->client->id, client_id) == 0) {
            return node;
        }
    }

    return NULL;
}

/**
 * Frees memory allocated by node structure. Calls free_client.
 */
void free_node(Node *node) {
    assert(node != NULL);

    free_client(node->client);
    free(node);
}
