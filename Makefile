all: client server

client: client.c p2pim.h
	gcc client.c -o client -g -O3 -std=c11 -D_POSIX_SOURCE

server: server.c p2pim.h
	gcc server.c -o server -g -O3 -std=c11 -D_POSIX_SOURCE
