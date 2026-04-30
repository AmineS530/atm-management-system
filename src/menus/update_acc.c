#include "menus.h"

void update_acc_info(User usr, sqlite3 *db)
{
    if (usr.accCount == 0)
    {
        show_error("No accounts found.");
        return;
    }

    int choice = select_account(usr);
    if (choice < 0) return;

    const char *options[] = {"Update Country", "Update Phone Number", "Back"};
    int prompt = show_menu("Update Account Information", options, 3);
    
    if (prompt >= 2) return;

    const char *query = (prompt == 0) ? "country" : "phone";
    char input[MAX_STR_LEN];
    
    while (1) {
        if (prompt == 0) { // Country
            get_input_field("Enter New Country (Letters and spaces only)", input, sizeof(input), 0);
            if (strlen(input) == 0) return;
            if (check_country(input)) break;
            show_error("Invalid country! Use letters and spaces only.");
        } else { // Phone
            get_input_field("New Phone: +<Code><Number> (Ex: +33123456)", input, sizeof(input), 0);
            if (strlen(input) == 0) return;
            if (check_phone_numb(input)) break;
            show_error("Invalid format! Use + followed by 9-18 digits.");
        }
    }

    char final_sql[200];
    snprintf(final_sql, sizeof(final_sql), "UPDATE records SET %s = ? WHERE accNbr = ?", query);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, final_sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        show_error("Database error.");
        return;
    }

    sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, usr.accountIds[choice]);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        show_error("Failed to update account.");
    } else {
        show_message("Account updated successfully.");
    }
    sqlite3_finalize(stmt);
}
