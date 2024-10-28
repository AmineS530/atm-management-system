#include "atm_sys.h"

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\tUser Login: ");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

// Function to retrieve the hashed password and return it

int checkPassword(sqlite3 *db, User *usr)
{
    const char *sql = "SELECT salt, passwd FROM users WHERE username = ?";
    sqlite3_stmt *stmt;
    unsigned char salt[SALT_SIZE];
    unsigned char stored_hashed_password[HASH_SIZE];
    unsigned char computed_hashed_password[HASH_SIZE] = {0}; // Store the hash of the user-entered password

    // Prepare the SQL query
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0; // Return 0 to indicate failure
    }

    // Bind the username from the User struct to the SQL query
    sqlite3_bind_text(stmt, 1, usr->name, -1, SQLITE_STATIC);

    // Execute the query and check if a row is returned
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Get the salt and stored hashed password from the database
        const void *salt_blob = sqlite3_column_blob(stmt, 0);
        const void *hashed_blob = sqlite3_column_blob(stmt, 1);

        int salt_size = sqlite3_column_bytes(stmt, 0);
        int hashed_size = sqlite3_column_bytes(stmt, 1);

        // Copy the salt and stored hashed password into local buffers
        memcpy(salt, salt_blob, salt_size);
        memcpy(stored_hashed_password, hashed_blob, hashed_size);

        // Hash the entered password with the retrieved salt
        hash_password((const char *)usr->password, salt, computed_hashed_password);

        // Compare the computed hash and the stored hash using memcmp
        if (memcmp(computed_hashed_password, stored_hashed_password, HASH_SIZE) == 0)
        {
            sqlite3_finalize(stmt); // Clean up the statement
            return 1;               // Password matches, return 1
        }
        else
        {
            sqlite3_finalize(stmt); // Clean up the statement
            return 0;               // Password does not match
        }
    }

    // No matching username found
    sqlite3_finalize(stmt); // Clean up
    return 0;               // Return 0 on failure
}