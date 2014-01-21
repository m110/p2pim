#pragma once

#include "common.h"
#include "structs.h"

struct OpcodeData {
    Opcode opcode;
    char *message;
    Node *list;
    Node *node;
    Client *client;
};
