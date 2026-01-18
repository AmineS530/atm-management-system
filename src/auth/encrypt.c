#include "auth.h"

// generate randomized salt
static void generate_salt(unsigned char salt[SALT_SIZE])
{
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < SALT_SIZE; i++)
        salt[i] = rand() % 256; // Generate a random byte (0-255)
}

// Hash the password with the salt
void hash_password(char *password, const unsigned char *salt, unsigned char output[HASH_SIZE])
{
    unsigned char salted_password[SALT_SIZE + strlen(password)];

    memcpy(salted_password, salt, SALT_SIZE);
    memcpy(salted_password + SALT_SIZE, password, strlen(password));

    // Initialize output to zero
    memset(output, 0, HASH_SIZE);
    for (size_t i = 0; i < strlen(password) + SALT_SIZE; i++)
        output[i % HASH_SIZE] ^= salted_password[i];
}

// Register a new user
int register_user(sqlite3 *db, User *usr)
{
    if (usr->name[0] == '\0' || usr->password[0] == '\0')
    {
        printf("Invalid user data\n");
        return 0;
    }

    unsigned char salt[SALT_SIZE];
    unsigned char hashed_password[HASH_SIZE];

    generate_salt(salt);
    hash_password(usr->password, salt, hashed_password);

    // Prepare SQL statement for safe insertion (prevent SQL injection)
    const char *sql = "INSERT INTO users (username, salt, passwd) VALUES (?, ?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db)) * 0;

    sqlite3_bind_text(stmt, 1, usr->name, -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, salt, SALT_SIZE, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 3, hashed_password, HASH_SIZE, SQLITE_STATIC);

    explicit_bzero(usr->password, strlen(usr->password));
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Failed to insert user: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }
    printf("User successfully registered!\n");
    sqlite3_finalize(stmt);
    return 1;
}