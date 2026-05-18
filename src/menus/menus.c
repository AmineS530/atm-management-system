#include "menus.h"

int login_menu(User *usr)
{
    while (1) {
        get_input_field("Account Login - Username (Empty to cancel)", usr->name, sizeof(usr->name), 0);
        if (strlen(usr->name) > 0) break;
        return -1;
    }

    while (1) {
        get_input_field("Account Login - Password (Empty to cancel)", usr->password, sizeof(usr->password), 1);
        if (strlen(usr->password) > 0) break;
        return -1;
    }
    return 1;
}

int register_menu(sqlite3 *db, User *usr)
{
    char pass[50];

invald_username:
    while (1) {
        get_input_field("New Registration - Username (Empty to cancel)", usr->name, MAX_STR_LEN, 0);
        if (strlen(usr->name) > 0) break;
        return -1;
    }

    if (username_exists(db, *usr))
    {
        const char *options[] = {"Retry", "Return to menu"};
        int choice = show_menu("Username already exists!", options, 2);
        if (choice == 0) goto invald_username;
        return -1;
    }

    while (1) {
        get_input_field("New Registration - Password (Empty to cancel)", usr->password, sizeof(usr->password), 1);
        if (strlen(usr->password) > 0) break;
        return -1;
    }

    while (1) {
        get_input_field("New Registration - Confirm Password (Empty to cancel)", pass, sizeof(pass), 1);
        if (strlen(pass) > 0) break;
        return -1;
    }

    if (strcmp(usr->password, pass) != 0) {
        show_error("Passwords do not match");
        return -1;
    }

    return 1;
}

void main_menu(sqlite3 *db, User usr)
{
    const char *options[] = {
        "Create a new account",
        "Update account information",
        "Check accounts",
        "Check list of owned accounts",
        "Make Transaction",
        "Remove existing account",
        "Transfer ownership",
        "Exit"
    };

    while (1)
    {
        char title[100];
        snprintf(title, sizeof(title), "Welcome %s - Main Menu", usr.name);
        int choice = show_menu(title, options, 8);

        switch (choice + 1)
        {
        case 1:
            create_new_acc(&usr, db);
            break;
        case 2:
            update_acc_info(usr, db);
            break;
        case 3:
            check_existing_accs(usr, db);
            break;
        case 4:
            check_all_accounts(usr, db);
            break;
        case 5:
            make_transaction(db, usr);
            break;
        case 6:
            remove_acc(db, &usr);
            break;
        case 7:
            transfer_ownership(db, &usr);
            break;
        case 8:
        case -1: // Fall through for 'q' or Exit
            return;
        default:
            break;
        }
    }
}

// first menu
int init_menu(sqlite3 *db, User *usr)
{
    if (!db || !usr)
        return INIT_MENU_EXIT;

    const char *options[] = {"Login", "Register", "Exit"};

    while (1)
    {
        int choice = show_menu("z1 ATM - Welcome", options, 3);
        switch (choice + 1)
        {
        case 1:
            if (login_menu(usr) == 1) {
                if (check_password(db, usr))
                {
                    show_message("Login successful!");
                    return 1;
                }
                else
                    show_error("Wrong password or Username");
            }
            break;
        case 2:
            if (register_menu(db, usr) == 1) {
                if (register_user(db, usr)) {
                    show_message("Account Created Successfully! Please login.");
                } else {
                    show_error("Failed to create account. Please try again.");
                }
            }
            break;
        case 3:
        case -1:
            return INIT_MENU_EXIT;
        }
    }
    return INIT_MENU_EXIT;
}