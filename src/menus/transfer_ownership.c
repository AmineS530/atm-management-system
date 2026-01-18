#include "menus.h"

static int get_recepient_uid(sqlite3 *db, char *username);

void transfer_ownership(sqlite3 *db, User *usr)
{
    if (usr->accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr->name);
        return;
    }
    printf("\t\t====== Transfer Ownership ======\n\n");
    int choice = select_account(*usr);
    const char *sql = "UPDATE records SET userID = ? WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    char username[20];
    int recepient_id = -1;
    while (1)
    {
        printf("\t\t====== Transfer Ownership ======\n\n");
        printf("Enter the username of the new owner: ");
        safe_string_input(username, sizeof(username));
        recepient_id = get_recepient_uid(db, username);
        if (recepient_id == -1)
        {
            printf("User not found!\n");
            continue;
        }
        if (recepient_id == usr->id)
        {
            printf("You cannot transfer ownership to yourself!\n");
            continue;
        }
        break;
    }

    sqlite3_bind_int(stmt, 1, recepient_id);
    sqlite3_bind_int(stmt, 2, usr->id);
    sqlite3_bind_int64(stmt, 3, usr->accountIds[choice]);
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Failed to update record: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }
    printf("\t\t====== Transfer Ownership ======\n\n");
    printf("\nRecord updated successfully.\n");
    sqlite3_finalize(stmt);
    get_acc_nbrs(usr, db);
}

static int get_recepient_uid(sqlite3 *db, char *username)
{
    const char *sql = "SELECT id FROM users WHERE username = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        return sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return -1;
}