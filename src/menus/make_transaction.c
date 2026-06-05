#include "menus.h"

static int get_acc_type(sqlite3 *db, User usr, int choice, char buffer[8]);
static int withdraw(sqlite3 *db, User usr, int choice, float balance);
static int deposit(sqlite3 *db, User usr, int choice, float balance);

void make_transaction(sqlite3 *db, User usr)
{
    if (usr.accCount == 0)
    {
        show_error("No accounts found.");
        return;
    }
    int choice = select_account(usr);
    if (choice < 0) return;

    char accType[8];
    if (!get_acc_type(db, usr, choice, accType)){
        show_error("Could not retrieve account type.");
        return;
    }

    if (strncasecmp(accType, "Fixed", 5) == 0)
    {
        show_error("ERROR: Cannot modify balance on FIXED accounts.");
        return;
    }

    float balance = get_balance(db, usr, choice);
    const char *options[] = {"Withdraw", "Deposit", "Back to menu"};

    while (1) {
        char title[100];
        snprintf(title, sizeof(title), "Transaction - Account: %ld", usr.accountIds[choice]);
        int prompt = show_menu(title, options, 3);

        if (prompt == 0) {
            if (withdraw(db, usr, choice, balance) == 0) break;
        } else if (prompt == 1) {
            if (deposit(db, usr, choice, balance) == 0) break;
        } else {
            break;
        }
    }
}

static int withdraw(sqlite3 *db, User usr, int choice, float balance)
{
    char input[20];
    char *endptr;

    while (1) {
        get_input_field("Withdraw - Enter amount", input, sizeof(input), 0);
        if (strlen(input) == 0) return -1;

        errno = 0;
        float withdrawAmount = strtof(input, &endptr);

        if (errno == ERANGE || *endptr != '\0' || withdrawAmount <= 0.0 || withdrawAmount > balance)
        {
            show_error("Invalid amount or insufficient balance.");
            continue;
        }

        const char *sql = "UPDATE records SET balance = ? WHERE userID = ? AND accNbr = ?";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;

        balance -= withdrawAmount;
        sqlite3_bind_double(stmt, 1, balance);
        sqlite3_bind_int(stmt, 2, usr.id);
        sqlite3_bind_int64(stmt, 3, usr.accountIds[choice]);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return -1;
        }
        sqlite3_finalize(stmt);

        char msg[100];
        snprintf(msg, sizeof(msg), "Withdrew %.2f. New balance: %.2f", withdrawAmount, balance);
        show_message(msg);
        return 0;
    }
}

static int deposit(sqlite3 *db, User usr, int choice, float balance)
{
    char input[20];
    char *endptr;

    while (1) {
        get_input_field("Deposit - Enter amount", input, sizeof(input), 0);
        if (strlen(input) == 0) return -1;

        errno = 0;
        float depositAmount = strtof(input, &endptr);

        if (errno == ERANGE || *endptr != '\0' || depositAmount >= MAX_BALANCE - balance || depositAmount <= 0)
        {
            show_error("Invalid amount.");
            continue;
        }

        const char *sql = "UPDATE records SET balance = ? WHERE userID = ? AND accNbr = ?";
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;

        balance += depositAmount;
        sqlite3_bind_double(stmt, 1, balance);
        sqlite3_bind_int(stmt, 2, usr.id);
        sqlite3_bind_int64(stmt, 3, usr.accountIds[choice]);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return -1;
        }
        sqlite3_finalize(stmt);

        char msg[100];
        snprintf(msg, sizeof(msg), "Deposited %.2f. New balance: %.2f", depositAmount, balance);
        show_message(msg);
        return 0;
    }
}
static int get_acc_type(sqlite3 *db, User usr, int choice, char buffer[8])
{
    const char *sql = "SELECT accType FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (!buffer || sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;

    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int64(stmt, 2, usr.accountIds[choice]);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *temp = (const char *)sqlite3_column_text(stmt, 0);
        if (temp && strlen(temp) < 8)
        {
            strcpy(buffer, temp);
            sqlite3_finalize(stmt);
            return 1;
        }
    }
    sqlite3_finalize(stmt);
    return 0;
}