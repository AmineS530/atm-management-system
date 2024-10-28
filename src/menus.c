#include "atm_sys.h"

void loginMenu(User *usr)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\tUser Login: ");
    scanf("%s", usr->name);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", usr->password);

    // restore terminal
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
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\tUser Login: ");
    scanf("%s", usr->name);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to the new account:");
    scanf("%s", usr->password);


    printf("\n\n\n\n\n\t\t\t\tRe-Enter the password:");
    scanf("%s", pass);

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

void initMenu(sqlite3 *db, User *usr)
{
    int r = 0;
    int option;
    char pass[50];

    system("clear");
    printOptions(2);
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(usr);
            if (checkPassword(db, usr))
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
            registerMenu(usr, pass);
            if (!strcmp(usr->password, pass) == 0)
            {
                exitErr("\n\t\t passwords do not match\n");
            }
            
            if (!username_exists(db, *usr))
            {
                registerUser(db, usr);
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