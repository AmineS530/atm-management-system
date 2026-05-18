#include "helpers.h"

void exit_err(char *str)
{
    log_error(NULL, "%s", str);
}