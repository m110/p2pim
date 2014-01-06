#include "p2pim.h"
#include "structs.h"

/* Linked list of clients. */
Client *client_list = NULL;

void free_client(Client *client) {
    free(client->id);
    free(client->addr);
    free(client);
}

/* Create new client structure */
Client* create_client(char *client_id, char *address, unsigned short port, struct sockaddr *addr) {
    Client *client = malloc(sizeof(Client));
    client->id = strdup(client_id);
    client->time = current_time();
   
    /* Save client's location */
    strcpy(client->public_addr.address, address);
    client->public_addr.port = port;

    /* Save connection info */
    client->addr = addr;

    client->next = NULL;

    return client;
}

int add_client(Client *client) {
    Client *c = client_list;
    for (; c != NULL && c->next != NULL; c = c->next);

    if (client_list == NULL) {
        client_list = client;
    } else {
        c->next = client;
    }

    return 0;
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
                return NULL;
            }
        }
    }

    return NULL;
}

Client* get_client_by_id(char *client_id) {
    for (Client *c = client_list; c != NULL; c = c->next) {
        if (strcmp(c->id, client_id) == 0) {
            if (current_time() - c->time <= CLIENT_DURATION) {
                return c;
            } else {
                delete_client(c);
                return NULL;
            }
        }
    }

    return NULL;
}
