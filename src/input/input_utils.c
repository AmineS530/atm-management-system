#include "input.h"

// silent validation for phone number
int check_phone_numb(char *phone)
{
    if (!phone) return 0;
    int inputLen = strlen(phone);
    
    if (inputLen < 9 || inputLen > 18)
        return 0;

    if (phone[0] != '+')
        return 0;

    for (int i = 1; i < inputLen; i++)
        if (phone[i] < '0' || phone[i] > '9')
            return 0;

    return 1;
}

int is_alpha(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(int c)
{
    return (c >= '0' && c <= '9');
}

// silent validation for country
int check_country(char *country)
{
    if (!country) return 0;
    size_t inputLen = strlen(country);

    if (inputLen < 2 || inputLen > 28)
        return 0;
    for (size_t i = 0; i < inputLen; i++)
        if (!is_alpha(country[i]) && country[i] != ' ')
            return 0;
    return 1;
}
