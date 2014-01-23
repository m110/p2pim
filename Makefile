
CFLAGS = -W -Wall -Werror -g -O3 -std=c11 -D_POSIX_SOURCE -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lpthread

SHARED_HEADERS = p2pim.h structs_common.h net.h opcodes.h tpl/tpl.h
SHARED_OBJS = p2pim.c structs_common.c net.c opcodes.c tpl/tpl.c

CLIENT = client
CLIENT_HEADERS = $(SHARED_HEADERS) opcodes_client.h
CLIENT_OBJS = $(CLIENT).c $(SHARED_OBJS) opcodes_client.c

SERVER = server
SERVER_HEADERS = $(SHARED_HEADERS) structs_server.h opcodes_server.h
SERVER_OBJS = $(SERVER).c $(SHARED_OBJS) structs_server.c opcodes_server.c

CC = gcc $(CFLAGS) $(LDFLAGS)

all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT_OBJS)
	$(CC) -DCLIENT=1 $(CLIENT_OBJS) -o $(CLIENT) -lncurses 

$(SERVER): $(SERVER_OBJS)
	$(CC) -DSERVER=1 $(SERVER_OBJS) -o $(SERVER)

clean:
	rm $(CLIENT) $(SERVER)
