#include "../atm_sys.h"

// **Make transaction** function
void make_transaction(sqlite3 *db, User usr)
{
    system("clear");
    if (usr.accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr.name);
        return;
    }
    int choice, prompt;
    choice = select_account(usr);
    char accType[8];
    printf("\n user id: %d, account id: %ld, choice: %d", usr.id, usr.accountIds[choice], choice);
    if (!get_acc_type(db, usr, choice, accType)){
        system("clear");
        printf("No accounts found for user: %s\n", usr.name);
        return;
    }
    printf("\n account type: %s\n", accType);
    float balance = get_balance(db, usr, choice);
invalid:
    prompt = -1;

    printf("\t\t====== Make Transaction ======\n\n");
    printf("\t\tselected account number: %ld\n", usr.accountIds[choice]);
    printf("\t\tOptions:\n\n\t\t"
           "[1] Withdraw\n\t\t"
           "[2] Diposit\n\n\t\t"
           "[3] Back to menu\n\n\t\t"
           "Your input: ");
        // printf("\n account type: %s , cmp: %d\n", *accType, strncasecmp(accType, "Fixed", 6));
    safe_int_input(&prompt);

    if (strncasecmp(accType, "Fixed", 5) == 0)
    {
        system("clear");
        printf("\t\t====== Make Transaction ======\n\n");
        printf("ERROR: Cannot modify balance on FIXED accounts.\n");
        sleep_sec(2);
        return;
    }
    if (prompt == 1)
        withdraw(db, usr, choice, balance);
    else if (prompt == 2)
        deposit(db, usr, choice, balance);
    else if (prompt == 3)
        main_menu(db, usr);
    else
    {
        system("clear");
        printf("\n   | [+] Non-Valid input |\n\n");
        goto invalid;
    }
    sleep_sec(2);
}

int withdraw(sqlite3 *db, User usr, int choice, float balance)
{
    const char *sql = "UPDATE records SET balance = ? WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    float withdrawAmount;
    system("clear");
    // todo: make transaction banner + withdrawing
    while (1)
    {
        printf("Enter amount to withdraw: ");
        if (!safe_float_input(&withdrawAmount) || withdrawAmount <= 0.0 || withdrawAmount > balance)
        {
            printf("%f\n balance: %f", withdrawAmount, balance);
            printf("✖ Invalid input! Please enter a valid amount.\n");
            continue;
        }
        break;
    }
    // Update balance
    balance -= withdrawAmount;

    // Bind parameters
    sqlite3_bind_double(stmt, 1, balance);
    sqlite3_bind_int(stmt, 2, usr.id);
    sqlite3_bind_int64(stmt, 3, usr.accountIds[choice]);

    // Execute
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    system("clear");
    // Print success message
    printf("\t\t\t===== Withdrawal Successful =====\n");
    printf("Account number: %ld\n", usr.accountIds[choice]);
    printf("Withdrawal amount: %.2f\n", withdrawAmount);
    printf("Old balance: %.2f New balance: %.2f\n", balance + withdrawAmount, balance);
    return 0;
}
int get_acc_type(sqlite3 *db, User usr, int choice, char buffer[8])
{
    const char *sql = "SELECT accType FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (!buffer || sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int64(stmt, 2, usr.accountIds[choice]);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *temp = (const char *)sqlite3_column_text(stmt, 0);
        if (temp && strlen(temp) < 8)
        {
            strcpy(buffer, temp);
            sqlite3_finalize(stmt);
            return true;
        }
    }

    sqlite3_finalize(stmt);
    return false;
}
int deposit(sqlite3 *db, User usr, int choice, float balance)
{
    const char *sql = "UPDATE records SET balance = ? WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;

    // Prepare statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Get deposit amount
    float depositAmount = 0;
    while (1)
    {
        printf("Enter amount to deposit: ");
        if (safe_float_input(&depositAmount) != 1 || depositAmount <= 0)
        {
            printf("✖ Invalid input! Please enter a valid amount.\n");
            continue;
        }
        break;
    }

    balance += depositAmount;

    sqlite3_bind_double(stmt, 1, balance);
    sqlite3_bind_int(stmt, 2, usr.id);
    sqlite3_bind_int64(stmt, 3, usr.accountIds[choice]);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    system("clear");
    // Print success message
    printf("\t\t\t===== Deposit Successful =====\n");
    printf("Account number: %ld\n", usr.accountIds[choice]);
    printf("Deposit amount: %.2f\n", depositAmount);
    printf("Old balance: %.2f New balance: %.2f\n", balance - depositAmount, balance);
    return 0;
}