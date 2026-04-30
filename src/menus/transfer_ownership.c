#include "menus.h"

static int get_recepient_uid(sqlite3 *db, char *username);

void transfer_ownership(sqlite3 *db, User *usr)
{
    if (usr->accCount == 0)
    {
        show_error("No accounts found.");
        return;
    }

    int choice = select_account(*usr);
    if (choice < 0) return;

    char username[20];
    int recepient_id = -1;

    while (1)
    {
        get_input_field("Transfer Ownership - Enter Recipient Username", username, sizeof(username), 0);
        if (strlen(username) == 0) return;

        recepient_id = get_recepient_uid(db, username);
        if (recepient_id == -1)
        {
            show_error("User not found!");
            continue;
        }
        if (recepient_id == usr->id)
        {
            show_error("You cannot transfer ownership to yourself!");
            continue;
        }
        break;
    }

    const char *sql = "UPDATE records SET userID = ? WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        show_error("Database error.");
        return;
    }

    sqlite3_bind_int(stmt, 1, recepient_id);
    sqlite3_bind_int(stmt, 2, usr->id);
    sqlite3_bind_int64(stmt, 3, usr->accountIds[choice]);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        show_error("Failed to update record.");
    } else {
        char msg[100];
        snprintf(msg, sizeof(msg), "Account %ld transferred to %s.", usr->accountIds[choice], username);
        show_message(msg);
    }
    sqlite3_finalize(stmt);
    get_acc_nbrs(usr, db);
}

static int get_recepient_uid(sqlite3 *db, char *username)
{
    const char *sql = "SELECT id FROM users WHERE username = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    int id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return id;
}