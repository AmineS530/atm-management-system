#include "atm_sys.h"

// check valid phone number input
int check_phone_numb(char *phone)
{
    printf("\n\n\t\tplease enter phone number in the following format:\n"
           "\t\t+<Country Code><phone number>\n\t\texample: +919876543210\n"
           "New Number:");
     if (!safeInput(phone)){
        return 0;
    }
    int inputLen = strlen(phone);
    if (inputLen < 9 || inputLen > 18)
        return printf("\n\n\t\tinvalid phone number\n") * 0;

    if (phone[0] != '+')
        return printf("\n\n\t\tphone number must start with +<Country code>\n") * 0;

    for (int i = 1; i < inputLen; i++)
        if (phone[i] < '0' || phone[i] > '9')
            return printf("\n\n\t\tphone number must only contain digits\n") * 0;

    return 1;
}


int is_alpha(int c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit(int c){
    return (c >= '0' && c <= '9');
}

int check_country(char *country)
{
    printf("\n\t\tPlease enter the country name:\n"
           "\t\tCountry: ");
    if (!safeInput(country)){
        return 0;
    }
    size_t inputLen = strlen(country);
    if (country[inputLen - 1] == '\n') {
        country[inputLen - 1] = '\0';
        inputLen--;
    }
    if (inputLen < 2 || inputLen > 28)
        return printf("\n\n\t\tError: Invalid country name\n") * 0;
    for (size_t i = 0; i < inputLen; i++)
        if (!is_alpha(country[i]) && country[i] != ' ')
            return printf("\n\n\t\tInvalid format: country name must only contain letters and spaces\n") * 0;
    return 1;
}
