#ifndef _CONNECT_H
#define _CONNECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

typedef struct _pozycja
{
    int x;
    int y;
    char type;
    char west;
    char east;
    char north;
    char south;
} pozycja;

typedef struct _pole
{
    int x;
    int y;
    char type;
} pole;

cJSON *transform(char *token, char *request, char *direction, cJSON *trash);
void explore(char *token, char *request, cJSON **exp, cJSON *trash);

#endif