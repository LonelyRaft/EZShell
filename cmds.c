
#include "ezshell.h"
#include <stdio.h>
#include <stdlib.h>

static void demo(int argc, char *argv[])
{
    printf("argc: %d\n", argc);
    for (int idx = 0; idx < argc; idx++)
        printf("%s\n", argv[idx]);
}
EZSHELL_REGISTER(demo)

static void ezexit(int argc, char *argv[])
{
    exit(0);
}
EZSHELL_REGISTER(ezexit);
