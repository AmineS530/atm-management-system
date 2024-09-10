#include "atm_sys.h"

void exitErr(char *str)
{
    system("clear");
    printf("%s", str);
    exit(1);
}