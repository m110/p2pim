#include "p2pim.h"

/* Get IP address from sockadd struct */
void get_address(struct sockaddr *sa, char *address) {
    if (sa->sa_family == AF_INET) {
        inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                address, INET_ADDRSTRLEN);
    } else {
        inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                address, INET6_ADDRSTRLEN);
    }
}

/* Get port number from sockaddr struct */
unsigned short get_port(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return ((struct sockaddr_in *)sa)->sin_port;
    } else {
        return ((struct sockaddr_in6 *)sa)->sin6_port;
    }
}
