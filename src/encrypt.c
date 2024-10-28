#include "atm_sys.h"

// Function to generate random salt using the rand() function
void generate_salt(unsigned char salt[SALT_SIZE])
{
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < SALT_SIZE; i++)
    {
        salt[i] = rand() % 256; // Generate a random byte (0-255)
    }
}

void hash_password(const char *password, const unsigned char *salt, unsigned char output[HASH_SIZE])
{
    unsigned char salted_password[SALT_SIZE + strlen(password)];

    // Combine salt and password
    memcpy(salted_password, salt, SALT_SIZE);
    memcpy(salted_password + SALT_SIZE, password, strlen(password));

    // Initialize output to zero
    memset(output, 0, HASH_SIZE);
    for (size_t i = 0; i < SALT_SIZE + strlen(password); i++)
    {
        output[i % HASH_SIZE] ^= salted_password[i];
    }
}

int compare_hashes(unsigned char hash1[HASH_SIZE], unsigned char hash2[HASH_SIZE])
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (hash1[i] != hash2[i])
        {
            return 0; // Passwords do not match
        }
    }
    return 1; // Passwords match
}

void registerUser(sqlite3 *db, User *usr)
{
    unsigned char salt[SALT_SIZE];
    unsigned char hashed_password[HASH_SIZE];

    // Generate a random salt
    generate_salt(salt);
    // Hash the password with the salt
    hash_password(usr->password, salt, hashed_password);

    // Prepare SQL statement for safe insertion (prevent SQL injection)
    const char *sql = "INSERT INTO users (username, salt, passwd) VALUES (?, ?, ?)";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind parameters to the SQL query
    sqlite3_bind_text(stmt, 1, usr->name, -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, salt, SALT_SIZE, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 3, hashed_password, HASH_SIZE, SQLITE_STATIC);

    // Execute the SQL query
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Failed to insert user: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("User successfully registered!\n");
    }

    // Clean up
    sqlite3_finalize(stmt);
}