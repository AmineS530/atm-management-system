#include "helpers.h"

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