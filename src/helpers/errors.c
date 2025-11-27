#include "../atm_sys.h"

void exit_err(char *str)
{
    system("clear");
    printf("%s", str);
    exit(1);
}