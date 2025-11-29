#include <stdio.h>
const MAX_ACCOUNTS = 8;
typedef struct s_User
{
    long accountIds[MAX_ACCOUNTS];
    int id;
    int accCount;
    char name[50];
    char password[50];
} User;

void print_user(const User *u)
{
    if (!u)
    {
        printf("User pointer is NULL\n");
        return;
    }

    printf("User Debug Info:\n");
    printf("  id         : %d\n", u->id);
    printf("  accCount   : %d\n", u->accCount);
    printf("  name       : \"%s\"\n", u->name);
    printf("  password   : \"%s\"\n", u->password);

    printf("  accountIds : ");
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        printf("%ld ", u->accountIds[i]);
    }
    printf("\n");
}
