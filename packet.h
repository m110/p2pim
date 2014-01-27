#ifndef _PACKET_H
#define _PACKET_H

#include "net.h"
#include "structs_common.h"
#include "opcodes.h"
#include "include/tpl.h"

#define MAX_MESSAGE_LEN         512

#define PACKET_TPL_FORMAT       "S(iiuc#)"
#define LIST_TPL_FORMAT         "A(ssv)"

struct packet_context {
    enum opcode opcode;
    enum opcode_status status;
    size_t message_len;
    char message[MAX_MESSAGE_LEN];
};

/* Packet preparation functions */
int prepare_packet_full(struct packet_context *p_ctx, enum opcode opcode,
        enum opcode_status status, char *message, size_t message_len);
int prepare_packet(struct packet_context *p_ctx, enum opcode opcode, char *message, size_t message_len);
int prepare_packet_message(struct packet_context *p_ctx, enum opcode opcode, char *message);
int prepare_packet_simple(struct packet_context *p_ctx, enum opcode opcode);
int prepare_packet_status(struct packet_context *p_ctx, enum opcode_status status);
void print_packet(char *prefix, struct packet_context *p_ctx);

/* Packet send functions */
int packet_send(int socket, struct peer *peer, struct packet_context *p_ctx);
int packet_recv(int socket, struct peer *peer, struct packet_context *p_ctx);
int packet_dialog(int socket, struct peer *peer, struct packet_context *p_ctx);

#endif
