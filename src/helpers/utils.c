#include "helpers.h"

// repeat spaces after welcome X for consistency
void print_accounts(Record rec)
{
    printf("_____________________\n");
    printf("Account number: %ld\n", rec.accountNbr);
    printf("Deposit Date: %s\n", rec.deposit);
    printf("Country: %s\n", rec.country);
    printf("Phone: %s\n", rec.phone);
    printf("Balance: %.2f\n", rec.balance);
    printf("Account Type: %s\n", rec.accountType);
    printf("_____________________\n");
}

char *to_upper(char *str)
{
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] -= 32;
    }
    return (str);
}

int select_account(User usr)
{
    if (usr.accCount == 0) return -1;

    const char **options = malloc(usr.accCount * sizeof(char *));
    for (int i = 0; i < usr.accCount; i++) {
        options[i] = malloc(50);
        snprintf((char *)options[i], 50, "Account: %ld", usr.accountIds[i]);
    }

    int choice = show_menu("Select Account", options, usr.accCount);

    for (int i = 0; i < usr.accCount; i++) {
        free((void *)options[i]);
    }
    free(options);

    return choice;
}

void sleep_sec(int seconds)
{
    if (seconds < 0)
        sleep(1);
    printf("\n\n");
    while (seconds-- > 0)
    {
        printf("\t\t [-] Redirecting in %d seconds...\n", seconds + 1);
        sleep(1);
    }
}