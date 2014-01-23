#ifndef _P2PIM_H
#define _P2PIM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#define DEFAULT_SERVER_PORT             5000

/* General utility functions */
unsigned int current_time();
char* strdup(const char *s);

#endif
