#include "atm_sys.h"

void printOptions(int input)
{
    if (input == 1)
        printf("\n\n\t\t============== ATM ==============\n\n"
               "\n\t\t-->> Feel free to choose one of the options below <<--\n"
               "\n\t\t[1]- Create a new account\n"
               "\n\t\t[2]- Update account information\n"
               "\n\t\t[3]- Check accounts\n"
               "\n\t\t[4]- Check list of owned account\n"
               "\n\t\t[5]- Make Transaction\n"
               "\n\t\t[6]- Remove existing account\n"
               "\n\t\t[7]- Transfer ownership\n"
               "\n\t\t[8]- Exit\n");

    if (input == 2)
        printf("\n\t\t============== ATM ==============\n"
               "\n\t\t-->> Feel free to login / register :\n"
               "\n\t\t[1]- login\n"
               "\n\t\t[2]- register\n"
               "\n\t\t[3]- exit\n");
    if (input == 3)
        printf("\n\n\t\t============== ATM SYS ==============\n"
               "\n\t      Thanks for using our ATM managment system\n\n");
}

void mainMenu(sqlite3 *db, User u)
{
    int option;
    system("clear");
    printOptions(1);
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(db, u);
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
        checkAllAccounts(db, &u);
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
        exitErr("\t\t\tExiting the program...");
    default:
        printf("Invalid operation!\n");
    }
}

void initMenu(sqlite3 *db, User *u)
{
    int r = 0;
    int option;
    system("clear");
    printOptions(2);
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (checkPassword(db, u))
            {
                printf("\n\nPassword Match!");
            }
            else
            {
                exitErr("\n\t\tWrong password!! or User Name\n");
            }
            r = 1;
            break;
        case 2:
            registerMenu(u->name, u->password);
            if (!username_exists(db, u->name))
            {
                registerUser(db, u);
            }
            else
            {
                exitErr("\n\t\t Please choose another username\n");
            }

            r = 1;
            break;
        case 3:
            exitErr("\t\t\tExiting the program...");
        default:
            system("clear");
            printf("\t\tPlease Insert a valid operation!\n");
            printOptions(2);
            break;
        }
    }
}

void forexit()
{
    printOptions(3);
}

int main()
{
    User u;

    sqlite3 *db;
    char *errMsg = 0;

    int resCode = sqlite3_open(DB_PATH, &db);
    if (resCode != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

   atexit(forexit);
    initMenu(db, &u);
   mainMenu(db, u);
    sqlite3_close(db);
    return 0;
}
