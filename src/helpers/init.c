#include "../atm_sys.h"

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
    char cwd[512];
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);
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
        fprintf(stderr, "%s\n", err);
        sqlite3_free(err);
        return 0;
    }
    return 1;
}

sqlite3 *init()
{
    if (sqlite3_open(DB_PATH, &db) != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    if (!table_exists(db, "users"))
        if (!exec_sql_file(db, SCHEMA_PATH))
        {
            fprintf(stderr, "Failed to load schema.sql\n");
            exit(EXIT_FAILURE);
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
    print_options(3, NULL);
}
