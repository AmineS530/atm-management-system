#include "atm_sys.h"

void loginMenu(User *usr)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\t\t\t\tAccount Login\n");
    printf("\n\t\t\tBank Management System\n\n\t\t[-] User Login: ");
    scanf(STRING_TO_SCAN, usr->name);

    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\t\t[-] Enter the password to login:");
    scanf(STRING_TO_SCAN, usr->password);

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

void registerMenu(User *usr, char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\t\t\t\tNew Account Registration\n");
    printf("\n\t\t\t\tBank Management System\n\t\t\t[-] Username: ");
    scanf(STRING_TO_SCAN, usr->name);

    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    printf("\n\t\t\t[-] Enter the password to the new account:");
    scanf(STRING_TO_SCAN, usr->password);

    printf("\n\t\t\t[-] Re-Enter the password:");
    scanf(STRING_TO_SCAN, pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

void mainMenu(sqlite3 *db, User u)
{
    int option;

    system("clear");
    printOptions(2, u.name);
    scanf("%d", &option);

    switch (option)
    {
    case 1:
      //  createNewAcc(db, u);
        break;
    case 2:
        //  TODO : add your **Update account information** function
        UpdateAccInfo(&u, db);
        break;
    case 3:
        //  TODO : add your **Check the details of existing accounts** function

        // void CheckExistingaccs(User u, sqlite3 *db)
        break;
    case 4:
        checkAllAccounts(db, &u);
        break;
    case 5:
        //  TODO : add your **Make transaction** function

        // MakeTransaction(User *usr, sqlite3 *db);
        break;
    case 6:
        // TODO : add your **Remove existing account** function

        // RemoveAcc(User *usr, sqlite3 *db);
        break;
    case 7:
        //  TODO : add your **Transfer owner** function

        // void TransferOwnership(User *usr, sqlite3 *db)
        break;
    case 8:
        exitErr("\t\t\tExiting the program...");
    default:
        printf("Invalid operation!\n");
    }
}

// first menu
void initMenu(sqlite3 *db, User *usr)
{
    int r = 0;
    int option;
    char pass[50];

    system("clear");
    printOptions(1, NULL);
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(usr);
            if (checkPassword(db, usr))
                printf("\n\nLogin successful!");
            else
                exitErr("\n\t\tWrong password!! or User Name\n");
            r = 1;
            break;
        case 2:
            registerMenu(usr, pass);
            if (!strcmp(usr->password, pass) == 0)
                exitErr("\n\t\t passwords do not match\n");
            if (!username_exists(db, *usr))
                registerUser(db, usr);
            else
                exitErr("\n\t\t Please choose another username\n");
            r = 1;
            break;
        case 3:
            exitErr("\t\t\tExiting the program...");
        default:
            system("clear");
            printf("\t\tPlease Insert a valid operation!\n");
            printOptions(1, NULL);
            break;
        }
    }
}