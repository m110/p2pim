#include "p2pim.h"

/* Get current time in seconds */
unsigned int current_time() {
    return (unsigned int) time(NULL);
}

/* String duplicate */
char* strdup (const char *s) {
    char *d = malloc(strlen(s) + 1);
    if (d != NULL) strcpy (d, s);
    return d;
}
