#include "helpers.h"

void get_acc_nbrs(User *usr, sqlite3 *db)
{
	sqlite3_stmt *stmt;
	usr->accCount = 0;
	if (usr == NULL)
	{
		log_error(NULL, "Error: User pointer is NULL.");
		return;
	}

	// get account ids
	const char *sql = "SELECT accNbr FROM records WHERE userID = ?";
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
	{
		log_error(usr->name, "Failed to prepare statement: %s", sqlite3_errmsg(db));
		return;
	}

	sqlite3_bind_int(stmt, 1, usr->id);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		long accNbr = sqlite3_column_int64(stmt, 0);
		usr->accountIds[usr->accCount] = accNbr;
		usr->accCount++;
	}
	sqlite3_finalize(stmt);
}

// Function to check if username already exists
int account_exists(long accNbr, sqlite3 *db)
{
	const char *sql = "SELECT COUNT(*) FROM records WHERE accNbr = ?";
	sqlite3_stmt *stmt;
	int exists = 0;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
	{
		log_error(NULL, "Failed to prepare statement: %s", sqlite3_errmsg(db));
		return -1;
	}

	sqlite3_bind_int64(stmt, 1, accNbr);

	if (sqlite3_step(stmt) == SQLITE_ROW)
		exists = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	return exists;
}

// get account balance
float get_balance(sqlite3 *db, User usr, int choice)
{
	if (!usr.accountIds[choice])
	{
		log_error(usr.name, "No accounts found for user: %s", usr.name);
		return -1;
	}

	const char *sql =
		"SELECT balance FROM records WHERE userID = ? AND accNbr = ?";
	sqlite3_stmt *stmt;
	float balance = 0.0;
	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
	{
		log_error(usr.name, "Failed to prepare statement: %s", sqlite3_errmsg(db));
		return balance;
	}
	sqlite3_bind_int(stmt, 1, usr.id);
	sqlite3_bind_double(stmt, 2, usr.accountIds[choice]);
	if (sqlite3_step(stmt) == SQLITE_ROW)
		balance = sqlite3_column_double(stmt, 0);

	sqlite3_finalize(stmt);
	return balance;
}

// Function to check if username already exists
int username_exists(sqlite3 *db, User usr)
{
	if (strlen(usr.name) == 0)
		return 0;
	const char *sql =
		"SELECT COUNT(*) FROM users WHERE LOWER(username) = LOWER(?)";
	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
	{
		log_error(usr.name, "Failed to prepare statement: %s", sqlite3_errmsg(db));
		return -1;
	}

	sqlite3_bind_text(stmt, 1, usr.name, -1, SQLITE_STATIC);

	int exists = 0;
	if (sqlite3_step(stmt) == SQLITE_ROW)
		exists = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);
	return exists > 0;
}
