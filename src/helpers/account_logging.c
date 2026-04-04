#include "helpers.h"

char *format_account(Record rec)
{
    char *buffer = malloc(256);
    if (!buffer) return NULL;

    snprintf(buffer, 256,
        "_____________________\n"
        "Account number: %ld\n"
        "Deposit Date: %s\n"
        "Country: %s\n"
        "Phone: %s\n"
        "Balance: %.2f\n"
        "Account Type: %s\n"
        "_____________________\n",
        rec.accountNbr,
        rec.deposit,
        rec.country,
        rec.phone,
        rec.balance,
        rec.accountType
    );

    return buffer;
}

Record get_record_by_accNbr(sqlite3 *db, long accNbr)
{
    sqlite3_stmt *stmt;
    Record rec;
    memset(&rec, 0, sizeof(Record));

    const char *sql =
        "SELECT id, userID, accNbr, fullname, country, phone, accType, balance, deposit_date "
        "FROM records WHERE accNbr = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return rec;

    sqlite3_bind_int64(stmt, 1, accNbr);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        rec.id = sqlite3_column_int(stmt, 0);
        rec.userId = sqlite3_column_int(stmt, 1);
        rec.accountNbr = sqlite3_column_int64(stmt, 2);

        snprintf(rec.name, sizeof(rec.name), "%s",
            sqlite3_column_text(stmt, 3));

        snprintf(rec.country, sizeof(rec.country), "%s",
            sqlite3_column_text(stmt, 4));

        snprintf(rec.phone, sizeof(rec.phone), "%s",
            sqlite3_column_text(stmt, 5));

        snprintf(rec.accountType, sizeof(rec.accountType), "%s",
            sqlite3_column_text(stmt, 6));

        rec.balance = sqlite3_column_double(stmt, 7);

        snprintf(rec.deposit, sizeof(rec.deposit), "%s",
            sqlite3_column_text(stmt, 8));
    }

    sqlite3_finalize(stmt);
    return rec;
}
