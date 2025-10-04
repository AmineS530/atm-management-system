#include "atm_sys.h"

// Safe string input
int safeStringInput(char *buffer, size_t size)
{
    if (fgets(buffer, size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
    return 1;
}

// Safe integer input
int safeIntInput(int *value)
{
    char temp[50];
    if (!safeStringInput(temp, sizeof(temp)))
        return 0;

    char *endptr;
    long val = strtol(temp, &endptr, 10);

    if (*endptr != '\0') // not a valid number
        return 0;

    *value = (int)val;
    return 1;
}
