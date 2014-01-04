#pragma once

#define CLIENT_DURATION 30

typedef struct Location {
    char address[INET6_ADDRSTRLEN];
    unsigned short port;
} Location;

typedef struct Client {
    char *id;
    unsigned int time;
    struct Location public_addr;
    struct sockaddr_storage *conninfo;
    struct Client *next;
} Client;

Client* create_client(char *client_id, char *address, unsigned short port, struct sockaddr_storage *conninfo);
int add_client(Client *client);
int delete_client(Client *client);
Client* get_client(char *address, unsigned short port);
Client* get_client_by_id(char *client_id);
void update_client(Client *client);
