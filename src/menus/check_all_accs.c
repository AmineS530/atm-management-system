// check all accounts for a user
#include "../atm_sys.h"

void check_all_accounts(User usr, sqlite3 *db)
{
    system("clear");
    if (usr.accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr.name);
        return;
    }

    const char *sql = "SELECT accNbr, created_at, country, phone, balance, accType FROM records WHERE userID = ?";
    sqlite3_stmt *stmt;

    printf("\t\t====== All accounts for user: %s =====\n\n", usr.name);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, usr.id);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Record info = {0};
        
        info.accountNbr = sqlite3_column_int64(stmt, 0);

        // COPY deposit safely into fixed-size array
        const unsigned char *deposit_text = sqlite3_column_text(stmt, 1);
        if (deposit_text)
        {
            strncpy(info.deposit, (const char *)deposit_text, sizeof(info.deposit) - 1);
            info.deposit[sizeof(info.deposit) - 1] = '\0';
        }

        // COPY country
        const unsigned char *country_text = sqlite3_column_text(stmt, 2);
        if (country_text)
        {
            strncpy(info.country, (const char *)country_text, sizeof(info.country) - 1);
            info.country[sizeof(info.country) - 1] = '\0';
        }

        // COPY phone
        const unsigned char *phone_text = sqlite3_column_text(stmt, 3);
        if (phone_text)
        {
            strncpy(info.phone, (const char *)phone_text, sizeof(info.phone) - 1);
            info.phone[sizeof(info.phone) - 1] = '\0';
        }

        // COPY accountType
        const unsigned char *type_text = sqlite3_column_text(stmt, 5);
        if (type_text)
        {
            strncpy(info.accountType, (const char *)type_text, sizeof(info.accountType) - 1);
            info.accountType[sizeof(info.accountType) - 1] = '\0';
        }

        // Balance
        info.balance = sqlite3_column_double(stmt, 4);

        print_accounts(info);
    }
    sqlite3_finalize(stmt);
}
