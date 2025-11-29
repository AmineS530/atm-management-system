#include "../atm_sys.h"

void print_options(int input, char *name)
{
    if (input == 1)
        printf("\n\t\t============== ATM ==============\n"
               "\n\t\t  -->>  login / register <<--\n"
               "\n\t\t[1]- login\n"
               "\n\t\t[2]- register\n"
               "\n\t\t[3]- exit\n");
    if (input == 2)
        printf("\n\t\t============== ATM ==============\n\t\t"
               "\n\t\t||\t Welcome %s!\t ||\n"
               "\n\t\t-->> Choose one of the options below <<--\n"
               "\n\t\t[1]- Create a new account\n"
               "\n\t\t[2]- Update account information\n"
               "\n\t\t[3]- Check accounts\n"
               "\n\t\t[4]- Check list of owned account\n"
               "\n\t\t[5]- Make Transaction\n"
               "\n\t\t[6]- Remove existing account\n"
               "\n\t\t[7]- Transfer ownership\n"
               "\n\t\t[8]- Exit\n",
               to_upper(name));
    if (input == 3)
        printf("\n\n\t\t============== ATM ==============\n"
               "\n\t\t The ATM System Have Been Closed\n");
}

void print_accounts(Record rec)
{
    printf("_____________________\n");
    printf("Account number: %ld\n", rec.accountNbr);
    printf("Created Date: %s\n", rec.deposit);
    printf("Country: %s\n", rec.country);
    printf("Phone: %s\n", rec.phone);
    printf("Balance: %.2f\n", rec.balance);
    printf("Account Type: %s\n", rec.accountType);
    printf("_____________________\n");
}

User init_user(void)
{
    User u;
    memset(&u, 0, sizeof(User));
    u.id = -1;
    return u;
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