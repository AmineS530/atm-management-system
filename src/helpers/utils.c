#include "helpers.h"

void print_options(int input, char *name)
{
    if (input == 1)
        printf("\n\t\t============== z1 ATM ==============\n"
               "\n\t\t  -->>  login / register <<--\n"
               "\n\t\t[1]- login\n"
               "\n\t\t[2]- register\n"
               "\n\t\t[3]- exit\n");
    if (input == 2)
        printf("\n\t\t============== z1 ATM ==============\n\t\t"
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
        printf("\n\n\t\t============== z1 ATM ==============\n"
               "\n\t\t The ATM System Have Been Closed\n");
}
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
    int choice = -1;

    system("clear");
    for (int i = 0; i < usr.accCount; i++)
        printf("[%d] Account number: %ld\n", i + 1, usr.accountIds[i]);

    while (1)
    {
        printf("Enter account number: ");

        if (safe_int_input(&choice) != 1)
        {
            printf("✖ Invalid input! Please enter a valid number.\n");
            continue;
        }

        if (choice < 1 || choice > usr.accCount)
        {
            printf("✖ Invalid option! Please enter a number between 1 and %d.\n",
                   usr.accCount);
            continue;
        }

        break;
    }
    system("clear");

    return choice - 1;
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