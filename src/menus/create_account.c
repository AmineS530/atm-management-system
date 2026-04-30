#include "menus.h"

const char *accountTypes[5] = {"Current", "Savings", "Fixed01", "Fixed02", "Fixed03"};

static Record fill_info(sqlite3 *db, User usr);

void create_new_acc(User *usr, sqlite3 *db)
{
    if (usr->accCount >= MAX_ACCOUNTS)
    {
        show_error("Maximum number of accounts reached.");
        return;
    }

    const char *sql = "INSERT INTO records (userID, accNbr, fullname, country, phone, balance, accType, deposit_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    Record info;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        show_error("Database error.");
        return;
    }

    info = fill_info(db, *usr);
    if (info.accountNbr == 0) {
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, usr->id);
    sqlite3_bind_int64(stmt, 2, info.accountNbr);
    sqlite3_bind_text(stmt, 3, info.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, info.country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, info.phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, info.balance);
    sqlite3_bind_text(stmt, 7, info.accountType, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, info.deposit, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        show_error("Failed to create account.");
    }
    else
    {
        show_message("New account created successfully!");
        get_acc_nbrs(usr, db);
    }
    sqlite3_finalize(stmt);
}

static Record fill_info(sqlite3 *db, User usr)
{
    Record info = {0};
    char buffer[50];
    char *endptr;

    info.userId = usr.id;

    // Date - Using a simple picker logic or strict string format
    while (1) {
        get_input_field("Creation Date (DD/MM/YYYY)", buffer, sizeof(buffer), 0);
        int d, m, y;
        if (sscanf(buffer, "%d/%d/%d", &d, &m, &y) == 3 && d >= 1 && d <= 31 && m >= 1 && m <= 12 && y >= 1900 && y <= 2100) {
            strncpy(info.deposit, buffer, sizeof(info.deposit) - 1);
            break;
        }
        show_error("Invalid date format. Use DD/MM/YYYY.");
    }

    // Account Number - Strict Digit check + strtol
    while (1) {
        get_input_field("Enter Account Number (Digits only)", buffer, sizeof(buffer), 0);
        if (strlen(buffer) == 0) continue;
        
        int all_digits = 1;
        for (int i = 0; buffer[i]; i++) {
            if (!is_digit(buffer[i])) { all_digits = 0; break; }
        }
        
        if (!all_digits) {
            show_error("Invalid characters! Use digits only.");
            continue;
        }

        errno = 0;
        long accNbr = strtol(buffer, &endptr, 10);
        if (errno == ERANGE || *endptr != '\0' || accNbr <= 0) {
            show_error("Invalid account number value.");
            continue;
        }

        if (account_exists(accNbr, db)) {
            show_error("Account number already exists.");
            continue;
        }
        info.accountNbr = accNbr;
        break;
    }

    // Name - Alpha + Spaces only
    while (1) {
        get_input_field("Enter Full Name", buffer, sizeof(buffer), 0);
        if (strlen(buffer) == 0) continue;

        int valid = 1;
        for (int i = 0; buffer[i]; i++) {
            if (!is_alpha(buffer[i]) && buffer[i] != ' ') { valid = 0; break; }
        }

        if (!valid) {
            show_error("Invalid name! Use letters and spaces only.");
            continue;
        }
        strncpy(info.name, buffer, sizeof(info.name) - 1);
        break;
    }

    // Phone - Use existing helper
    while (1) {
        get_input_field("Phone: +<Country Code><Number> (Ex: +33123456)", buffer, sizeof(buffer), 0);
        if (check_phone_numb(buffer)) {
            strncpy(info.phone, buffer, sizeof(info.phone) - 1);
            break;
        }
        show_error("Invalid format! Use + followed by 9-18 digits.");
    }

    // Country - Use existing helper
    while (1) {
        get_input_field("Enter Country (Letters and spaces only)", buffer, sizeof(buffer), 0);
        if (check_country(buffer)) {
            strncpy(info.country, buffer, sizeof(info.country) - 1);
            break;
        }
        show_error("Invalid country! Use letters and spaces only.");
    }

    // Account Type
    const char *types[] = {
        "Current",
        "Savings (7%)",
        "Fixed01 (4%)",
        "Fixed02 (5%)",
        "Fixed03 (8%)"
    };
    int type_choice = show_menu("Select Account Type", types, 5);
    if (type_choice < 0) type_choice = 0;
    strncpy(info.accountType, accountTypes[type_choice], sizeof(info.accountType) - 1);

    // Balance - Strict strtod
    while (1) {
        get_input_field("Enter Initial Balance", buffer, sizeof(buffer), 0);
        errno = 0;
        double bal = strtod(buffer, &endptr);
        if (errno == ERANGE || *endptr != '\0' || bal < 0) {
            show_error("Invalid balance amount.");
            continue;
        }
        info.balance = bal;
        break;
    }

    return info;
}
