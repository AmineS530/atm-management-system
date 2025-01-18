#include "atm_sys.h"

//  gets salt and hashed password from db and compares it with the user entered password
int checkPassword(sqlite3 *db, User *usr)
{
    const char *sql = "SELECT id, salt, passwd FROM users WHERE username = ?";
    sqlite3_stmt *stmt;
    unsigned char salt[SALT_SIZE];
    unsigned char stored_hashed_password[HASH_SIZE];
    unsigned char computed_hashed_password[HASH_SIZE] = {0}; // Store the hash of the user-entered password

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, usr->name, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        usr->id = sqlite3_column_int(stmt, 0);
        const void *salt_blob = sqlite3_column_blob(stmt, 1);
        const void *hashed_blob = sqlite3_column_blob(stmt, 2);

        int salt_size = sqlite3_column_bytes(stmt, 1);
        int hashed_size = sqlite3_column_bytes(stmt, 2);

        memcpy(salt, salt_blob, salt_size);
        memcpy(stored_hashed_password, hashed_blob, hashed_size);

        hash_password(usr->password, salt, computed_hashed_password);

        // Compare the computed hash and the stored hash using memcmp
        if (memcmp(computed_hashed_password, stored_hashed_password, HASH_SIZE) == 0)
        {
            sqlite3_finalize(stmt);
            getAccNbrs(usr, db);
            return 1;
        }
    }
    sqlite3_finalize(stmt);
    return 0;
}