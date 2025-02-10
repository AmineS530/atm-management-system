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

void registerMenu(sqlite3 *db, User *usr)
{
    struct termios oflags, nflags;
    char pass[50];
    int err_prompt = 0;

invald_username:
    system("clear");
    printf("\n\n\t\t\t\tNew Account Registration\n");
    printf("\n\t\t\t\tBank Management System\n\t\t\t[-] Username: ");
    scanf(STRING_TO_SCAN, usr->name);
    if (username_exists(db, *usr))
    {
        system("clear");
        printf("\n\t\t\t[-] Username already exists!\n");
    invalid:
        printf("\n\t\t\t[-] enter 0 to retry or 1 to return to menu: ");
        scanf("%d", &err_prompt);
        switch (err_prompt)
        {
            case 0:
                sleep_sec(3);
                goto invald_username;
                break;
            case 1:
                sleep_sec(3);
                return initMenu(db, usr);
                break;
            default:
                printf("Insert a valid operation!\n");
                goto invalid;
        }
    }

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
       if (!strcmp(usr->password, pass) == 0)
                exitErr("\n\t\t[-] Passwords do not match\n");

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

    while (1)
    {
        printOptions(2, u.name);
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            createNewAcc(u, db);
            success(db, u);
            break;
        case 2:
            UpdateAccInfo(u, db);
            success(db, u);
            break;
        case 3:
            CheckExistingaccs(u, db);
            success(db, u);
            break;
        case 4:
            checkAllAccounts(u, db);
            success(db, u);
            break;
        case 5:
            //  TODO : add your **Make transaction** function
            // MakeTransaction(User *usr, sqlite3 *db);
            success(db, u);
            break;
        case 6:
            // TODO : add your **Remove existing account** function
            // RemoveAcc(User *usr, sqlite3 *db);
            success(db, u);
            break;
        case 7:
            //  TODO : add your **Transfer owner** function
            // void TransferOwnership(User *usr, sqlite3 *db)
            success(db, u);
            break;
        case 8:
            exitErr("\t\t\tExiting the program...");
            break;
        default:
            system("clear");
            clear_buffer();
            printf("Invalid operation!\n");
        }
    }
}

// first menu
void initMenu(sqlite3 *db, User *usr)
{
    int r = 0;
    int option;

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
            registerMenu(db ,usr);
            if (registerUser(db, usr)) {
                    just_a_menu();
                    initMenu(db, usr);
                }
            r = 1;
            break;
        case 3:
            exitErr("\t\t\t[-] Exiting the program...");
            break;
        default:
            system("clear");
            clear_buffer();
            printf("\t\tPlease Insert a valid operation!\n");
            printOptions(1, NULL);
            break;
        }
    }
}

void just_a_menu()
{
    system("clear");
    printf("\n\t\t============== ATM ==============\n"
           "\n\t\t  Account Created Successfully\n"
           "\n\t Login to acess your new account\n\n");
    sleep_sec(3);
}

void sleep_sec(int seconds)
{
    if (seconds < 0)
        sleep(1);
    while (seconds-- > 0)
    {
        printf("\t\t [-] Redirecting in %d seconds...\n", seconds+1);
        sleep(1);
    }
}