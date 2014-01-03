#include "p2pim.h"
#include "structs.h"

/* Linked list of clients. */
Client *client_list = NULL;

void free_client(Client *client) {
    free(client->id);
    free(client);
}

void add_client(char *client_id, char *address, unsigned short port) {
    Client *c = client_list;
    for (; c != NULL && c->next != NULL; c = c->next);

    /* Create new client */
    Client *new_client = malloc(sizeof(Client));
    new_client->id = strdup(client_id);
    new_client->time = current_time();
    
    /* Save client's location */
    strcpy(new_client->public_addr.address, address);
    new_client->public_addr.port = port;

    new_client->next = NULL;

    if (client_list == NULL) {
        client_list = new_client;
    } else {
        c->next = new_client;
    }
}

int delete_client(Client *client) {
    if (client == client_list) {
        client_list = client->next;
        free_client(client);
        return 0;
    }

    for (Client *c = client_list; c != NULL; c = c->next) {
        if (c->next == client) {
            c->next = client->next;
            free_client(client);
            return 0;
        }
    }

    return 1;
}

void update_client(Client *client) {
    client->time = current_time();
}

Client* get_client(char *address, unsigned short port) {
    for (Client *c = client_list; c != NULL; c = c->next) {
        if (strcmp(c->public_addr.address, address) == 0 &&
            c->public_addr.port == port) {

            if (current_time() - c->time <= CLIENT_DURATION) {
                return c;
            } else {
                delete_client(c);
            }
        }
    }

    return NULL;
}

Client* get_client_by_id(char *client_id) {
    for (Client *c = client_list; c != NULL; c = c->next) {
        if (strcmp(c->id, client_id) == 0) {
            return c;
        }
    }

    return NULL;
}
