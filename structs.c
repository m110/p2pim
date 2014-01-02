#include "p2pim.h"
#include "structs.h"

void free_client(Client *client) {
    free(client->id);
    free(client);
}

void add_client(Client **list, char *client_id) {
    Client *c = *list;
    for (; c != NULL && c->next != NULL; c = c->next);

    Client *new_client = malloc(sizeof(Client));
    new_client->id = strdup(client_id);
    new_client->time = current_time();
    new_client->next = NULL;

    if (*list == NULL) {
        *list = new_client;
    } else {
        c->next = new_client;
    }
}

int delete_client(Client **list, Client *client) {
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

Client* find_client(Client *list, char *client_id) {
    for (Client *c = list; c != NULL; c = c->next) {
        if (strcmp(c->id, client_id) == 0) {
            return c;
        }
    }

    return NULL;
}
