
CFLAGS = -g -O3 -std=c11 -D_POSIX_SOURCE
LDFLAGS = -lpthread

CLIENT = client
CLIENT_HEADERS = p2pim.h opcodes.h
CLIENT_OBJS = $(CLIENT).c p2pim.c opcodes.c opcodes_client.c

SERVER = server
SERVER_HEADERS = p2pim.h structs.h opcodes.h
SERVER_OBJS = $(SERVER).c p2pim.c structs.c opcodes_server.c opcodes.c

CC = gcc $(CFLAGS) $(LDFLAGS)

all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $(CLIENT) -lncurses 

$(SERVER): $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $(SERVER)

clean:
	rm $(CLIENT) $(SERVER)
