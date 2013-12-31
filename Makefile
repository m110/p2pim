all: client server

client: client.c p2pim.c p2pim.h
	gcc client.c p2pim.c -o client -g -O3 -std=c11 -D_POSIX_SOURCE -lpthread

server: server.c p2pim.c structs.c p2pim.h structs.h
	gcc server.c p2pim.c structs.c -o server -g -O3 -std=c11 -D_POSIX_SOURCE -lpthread
