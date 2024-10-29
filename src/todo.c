#include "atm_sys.h"

//  TODO : add your **Update account information** function
void UpdateAccInfo(User *usr, sqlite3 *db)
{
    const char *sql = "SELECT country, phone FROM records WHERE username = ?";
    sqlite3_stmt *stmt;

    // Prepare the SQL query
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, usr->name, -1, SQLITE_STATIC);



    sqlite3_finalize(stmt);
}

// TODO : add your **Make transaction** function
void MakeTransaction(User *usr, sqlite3 *db)
{

}

// TODO : add your **Transfer owner** function
void TransferOwnership(User *usr, sqlite3 *db)
{

}

// TODO : add your **Check the details of existing accounts** function
void CheckExistingaccs(User u, sqlite3 *db)
{

}

// TODO : add your **Remove existing account** function
void RemoveAcc(User *usr, sqlite3 *db)
{
    
}