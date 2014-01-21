
CFLAGS = -g -O3 -std=c11 -D_POSIX_SOURCE
LDFLAGS = -lpthread

CLIENT = client
CLIENT_HEADERS = p2pim.h structs_common.h net.h opcodes.h opcodes_client.h
CLIENT_OBJS = $(CLIENT).c p2pim.c structs_common.c net.c opcodes.c opcodes_client.c

SERVER = server
SERVER_HEADERS = p2pim.h structs_common.h net.h structs_server.h opcodes.h opcodes_server.h
SERVER_OBJS = $(SERVER).c p2pim.c structs_common.c net.c structs_server.c opcodes_server.c opcodes.c

CC = gcc $(CFLAGS) $(LDFLAGS)

all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT_OBJS)
	$(CC) -DCLIENT=1 $(CLIENT_OBJS) -o $(CLIENT) -lncurses 

$(SERVER): $(SERVER_OBJS)
	$(CC) -DSERVER=1 $(SERVER_OBJS) -o $(SERVER)

clean:
	rm $(CLIENT) $(SERVER)
