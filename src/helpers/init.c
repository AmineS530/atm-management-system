#include "helpers.h"

static sqlite3 *db = NULL;
static void forexit();

/* check if a table exists */
static int table_exists(sqlite3 *db, const char *name)
{
    const char *q =
        "SELECT 1 FROM sqlite_master WHERE type='table' AND name=? LIMIT 1;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, q, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    int exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

/* load SQL file and exec it */
static int exec_sql_file(sqlite3 *db, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *sql = malloc(size + 1);
    if (!sql)
    {
        fclose(f);
        return 0;
    }

    fread(sql, 1, size, f);
    fclose(f);
    sql[size] = '\0';

    char *err = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err);
    free(sql);

    if (rc != SQLITE_OK)
    {
        log_error(NULL, "%s", err);
        sqlite3_free(err);
        return 0;
    }
    return 1;
}

sqlite3 *init()
{
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK)
    {
        log_error(NULL, "error: %s", sqlite3_errmsg(db));
    }
    if (!table_exists(db, "users"))
        if (!exec_sql_file(db, SCHEMA_PATH))
        {
            log_error(NULL, "Failed to load schema.sql");
        }

    atexit(forexit);
    return db;
}

User init_user(void)
{
    User u;
    memset(&u, 0, sizeof(User));
    u.id = -1;
    return u;
}

static void forexit()
{
    sqlite3_close(db);
    printf("\n\n\t\t============== z1 ATM ==============\n"
              "\n\t\t   The ATM System Has Been Closed"
           "\n\n\t\t====================================\n\n");
}
