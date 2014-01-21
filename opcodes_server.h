#pragma once

#include "structs_server.h"

struct OpcodeData {
    Opcode opcode;
    char *message;
    Node *list;
    Node *node;
    Client *client;
};
