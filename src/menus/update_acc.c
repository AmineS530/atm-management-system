#include "../atm_sys.h"

//  **Update account information** function
void update_acc_info(User usr, sqlite3 *db)
{
    if (usr.accCount == 0)
    {
        system("clear");
        printf("No accounts found for user: %s\n", usr.name);
        return;
    }

    char input[30];
    sqlite3_stmt *stmt;
    int prompt, choice;
    int err = 0;
    char sql[MAX_STR_LEN], sqlold[MAX_STR_LEN];
    const char *query = NULL;
    char old_val[MAX_STR_LEN];
    memset(old_val, 0, MAX_STR_LEN);
    system("clear");

    for (int i = 0; i < usr.accCount; i++)
        printf("[%d] Account number: %ld\n", i + 1, usr.accountIds[i]);

    while (1)
    {
        choice = -1;
        printf("Enter account number: ");
        if (safe_int_input(&choice) != 1)
            printf("✖ Invalid input! Please enter a valid number.\n");
        else if (choice < 1 || choice > (usr.accCount))
        {
            printf("✖ Invalid option! Please enter a number between 1 and %d.\n", usr.accCount);
        }
        else
            break;
    }
    system("clear");
invalid:
    prompt = -1;

    printf("\t\t====== Update Account Informations ======\n\n");
    printf("\t\tselected account number: %ld\n", usr.accountIds[choice - 1]);
    printf("\t\tOptions:\n\n\t\t"
           "[1] Update Country\n\t\t"
           "[2] Update Phone-Number\n\n\t\t"
           "[3] Exit\n\n\t\t"
           "Your input: ");
    safe_int_input(&prompt);
    if (prompt == 1)
    {
        query = "country";
        do
        {
            err = check_country(input);
            system("clear");
            printf("\t\t\t===== New record =====\n");
        } while (err != 1);
    }
    else if (prompt == 2)
    {
        query = "phone";
        do
        {
            err = check_phone_numb(input);
            system("clear");
            printf("\t\t\t===== New record =====\n");
        } while (err != 1);
    }
    else if (prompt == 3)
        main_menu(db, usr);
    else
    {
        system("clear");
        // can store this in a var that start empty so its appear under update acc info txt or re add text here
        printf("\n   | [+] Non-Valid input |\n\n");
        goto invalid;
    }
    snprintf(sqlold, sizeof(sqlold), "SELECT %s FROM records WHERE accNbr = ?", query);
    if (sqlite3_prepare_v2(db, sqlold, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_int64(stmt, 1, usr.accountIds[choice]);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *old = sqlite3_column_text(stmt, 0);
        strncpy(old_val, (const char *)old, sizeof(old_val) - 1);
    }
    sqlite3_finalize(stmt);
    snprintf(sql, sizeof(sql), "UPDATE records SET %s = ? WHERE accNbr = ?", query);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, usr.accountIds[choice]);
    if (sqlite3_step(stmt) != SQLITE_DONE || err == 0)
    {
        system("clear");
        fprintf(stderr, "\t\tExecution failed\n");
    }
    sqlite3_finalize(stmt);
    printf("Account information updated successfully!\n"
           "old value: %s\n"
           "new value: %s\n",
           old_val, input);
    sleep_sec(3);
}
