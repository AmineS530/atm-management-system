#include "header.h"

void mainMenu(User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n"
           "\n\t\t-->> Feel free to choose one of the options below <<--\n"
           "\n\t\t[1]- Create a new account\n"
           "\n\t\t[2]- Update account information\n"
           "\n\t\t[3]- Check accounts\n"
           "\n\t\t[4]- Check list of owned account\n"
           "\n\t\t[5]- Make Transaction\n"
           "\n\t\t[6]- Remove existing account\n"
           "\n\t\t[7]- Transfer ownership\n"
           "\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        // here
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
}

void initMenu(User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n"
           "\n\t\t-->> Feel free to login / register :\n"
           "\n\t\t[1]- login\n"
           "\n\t\t[2]- register\n"
           "\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                printf("\n\nPassword Match!");
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
            // student TODO : add your **Registration** function
            // here
            r = 1;
            break;
        case 3:
            exit(1);
        default:
            printf("Insert a valid operation!\n");
            break;
        }
    }
}

void forexit()
{
    system("clear");
    printf("\n\t\t============== ATM SYS ==============\n"
    "\n\t      Thanks for using out ATM managment system\n\n");
}

int main()
{
    User u;

    atexit(forexit);
    initMenu(&u);
    mainMenu(u);
    return 0;
}
