#include "p2pim.h"
#include "structs.h"

void free_client(Client *client) {
    free(client->id);
    free(client->sockaddr);
    free(client);
}

int add_client(List **list, Client *client) {
    Client *c = *list;
    for (; c != NULL && c->next != NULL; c = c->next);

    if (*list == NULL) {
        *list = client;
    } else {
        c->next = client;
    }

    client->next = NULL;
    return 0;
}

int delete_client(List **list, Client *client) {
    if (client == *list) {
        *list = client->next;
        free_client(client);
        return 0;
    }

    for (Client *c = *list; c != NULL; c = c->next) {
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

Client* get_client(List *list, char *address, unsigned short port) {
    for (Client *c = *list; c != NULL; c = c->next) {
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

Client* get_client_by_id(List *list, char *client_id) {
    for (Client *c = *list; c != NULL; c = c->next) {
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
