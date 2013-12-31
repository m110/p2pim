#pragma once

typedef struct Location {
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
} Location;

typedef struct Client {
    char *id;
    unsigned int time;
    struct Location public_addr;
    struct Client *next;
} Client;

void add_client(Client **list, char *client_id);
int delete_client(Client **list, Client *client);
Client* find_client(Client *list, char *client_id);
