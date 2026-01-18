#include "helpers.h"

void exit_err(char *str)
{
    system("clear");
    printf("%s", str);
    exit(1);
}