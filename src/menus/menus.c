#include "../atm_sys.h"

void login_menu(User *usr)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\t\t\t\tAccount Login\n");
    printf("\n\t\t\tBank Management System\n\n\t\t[-] User Login: ");
    safe_string_input(usr->name, sizeof(usr->name));

    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
    printf("\n\t\t[-] Enter the password to login:");
    safe_string_input(usr->password, 0);

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }
}

int register_menu(sqlite3 *db, User *usr)
{
    struct termios oflags, nflags;
    char pass[50];
    int err_prompt = 0;

invald_username:
    system("clear");
    printf("\n\n\t\t\t\tNew Account Registration\n");
    printf("\n\t\t\t\tBank Management System\n\t\t\t[-] Username: ");
    safe_string_input(usr->name, MAX_STR_LEN);
    if (username_exists(db, *usr))
    {
        system("clear");
        printf("\n\t\t\t[-] Username already exists!\n");
    invalid:
        printf("\n\t\t\t[-] enter 0 to retry or 1 to return to menu: ");
        safe_int_input(&err_prompt);
        switch (err_prompt)
        {
        case 0:
            sleep_sec(3);
            goto invald_username;
            break;
        case 1:
            sleep_sec(3);
            return -1;
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
        exit(1);
    }

    printf("\n\t\t\t[-] Enter the password to the new account:");
    safe_string_input(usr->password, 0);

    printf("\n\t\t\t[-] Re-Enter the password:");
    safe_string_input(pass, 0);
    if (!strcmp(usr->password, pass) == 0)
        exit_err("\n\t\t[-] Passwords do not match\n");

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(1);
    }

    return 1;
}

void main_menu(sqlite3 *db, User u)
{
    int option;

    system("clear");
    while (1)
    {
        print_options(2, u.name);
        safe_int_input(&option);
        switch (option)
        {
        case 1:
            create_new_acc(u, db);
            success(db, u);
            break;
        case 2:
            update_acc_info(u, db);
            success(db, u);
            break;
        case 3:
            check_existing_accs(u, db);
            success(db, u);
            break;
        case 4:
            check_all_accounts(u, db);
            success(db, u);
            break;
        case 5:
            //  TODO : add your **Make transaction** function
            make_transaction(db, u);
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
            exit_err("\t\t\tExiting the program...");
            break;
        default:
            system("clear");
            printf("Invalid operation!\n");
        }
    }
}

// first menu
int init_menu(sqlite3 *db, User *usr)
{
    // int r = 0;
    int option;

    system("clear");
    print_options(1, NULL);
    while (1)
    {
        safe_int_input(&option);
        switch (option)
        {
        case 1:
            login_menu(usr);
            if (check_password(db, usr))
            {
                printf("\n\nLogin successful!\n");
                return 1;
            }
            else
                exit_err("\n\t\tWrong password!! or User Name\n"); // to replace with repromt
            // r = 1;
            break;
        case 2:
            if (register_menu(db, usr) == 1)
                if (register_user(db, usr))
                {
                    just_a_menu();
                    login_menu(usr);
                    // r = 1;
                    return 1;
                }
            option = -1;
            break;
        case 3:
            exit_err("\t\t\t[-] Exiting the program...");
            break;
        default:
            system("clear");
            printf("\t\tPlease Insert a valid operation!\n");
            print_options(1, NULL);
            break;
        }
    }
    return 0;
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
        printf("\t\t [-] Redirecting in %d seconds...\n", seconds + 1);
        sleep(1);
    }
}