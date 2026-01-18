#include "menus.h"

void login_menu(User *usr)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\t\t\t\tAccount Login\n");
    printf("\n\t\t\tBank Management System\n\n\t\t[-] User Login: ");
    if (!safe_string_input(usr->name, sizeof(usr->name)))
    {
        exit(1);
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
    printf("\n\t\t[-] Enter the password to login:");
    if (!safe_string_input(usr->password, 0))
    {
        exit(1);
    }

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
    if (!safe_string_input(usr->name, MAX_STR_LEN))
    {
        exit(1);
    }
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
    if (!safe_string_input(usr->password, 0))
    {
        exit(1);
    }

    printf("\n\t\t\t[-] Re-Enter the password:");
    if (!safe_string_input(pass, 0))
    {
        exit(1);
    }
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

void main_menu(sqlite3 *db, User usr)
{
    int option = -1;

    system("clear");
    while (1)
    {
        print_options(2, usr.name);
        safe_int_input(&option);
        switch (option)
        {
        case 1:
            create_new_acc(usr, db);
            success(db, usr);
            break;
        case 2:
            update_acc_info(usr, db);
            success(db, usr);
            break;
        case 3:
            check_existing_accs(usr, db);
            success(db, usr);
            break;
        case 4:
            check_all_accounts(usr, db);
            success(db, usr);
            break;
        case 5:
            make_transaction(db, usr);
            success(db, usr);
            break;
        case 6:
            remove_acc(db, &usr);
            success(db, usr);
            break;
        case 7:
            transfer_ownership(db, &usr);
            success(db, usr);
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
    if (!db || !usr)
        return INIT_MENU_EXIT;

    int option = -1;

    system("clear");
    print_options(1, NULL);
    while (1)
    {
        if (!safe_int_input(&option))
        {
            fprintf(stderr, "Input error. Exiting.\n");
            return INIT_MENU_EXIT;
        }
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
                printf("\n\t\tWrong password!! or User Name\n"); // to replace with repromt
            break;
        case 2:
            if (register_menu(db, usr) == 1)
                if (register_user(db, usr))
                {
                    just_a_menu();
                    login_menu(usr);
                    return INIT_MENU_GO;
                }
            break;
        case 3:
            exit_err("\t\t\t[-] Exiting the program...");
            break;
        default:
            system("clear");
            printf("\t\tPlease Insert a valid operation!\n");
            print_options(1, NULL);
        }
    }
    return INIT_MENU_EXIT;
}

void just_a_menu()
{
    system("clear");
    printf("\n\t\t============== ATM ==============\n"
           "\n\t\t  Account Created Successfully\n"
           "\n\t Login to acess your new account\n\n");
    sleep_sec(3);
}