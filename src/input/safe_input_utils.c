#include "../atm_sys.h"

// Safe string input
int safe_string_input(char *buffer, size_t size)
{
    if (size == 0)
    {
        size = MAX_STR_LEN;
    }

    if (fgets(buffer, size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return 0;
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
    return 1;
}

// Safe integer input
int safe_int_input(int *value)
{
    char temp[50];
    if (!safe_string_input(temp, sizeof(temp)))
        return 0;

    char *endptr;
    long val = strtol(temp, &endptr, 10);

    if (*endptr != '\0') // not a valid number
        return 0;

    *value = (int)val;
    return 1;
}

// Safe float input
int safe_float_input(float *value)
{
    char temp[50];
    if (!safe_string_input(temp, sizeof(temp)))
        return 0;

    char *endptr;
    float val = strtof(temp, &endptr);

    // Check if *anything* was parsed
    if (endptr == temp)
        return 0;

    // Check for leftover garbage characters
    if (*endptr != '\0')
        return 0;

    *value = val;
    return 1;
}
