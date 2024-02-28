#ifndef FATAL_H
#define FATAL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR(msg)                                                                    \
    fprintf(stderr, "%s:%d FATAL ERROR[%s]:%s\n", __FILE__, __LINE__, __func__, msg); \
    exit(EXIT_FAILURE);                                                               \

#endif // FATAL_H