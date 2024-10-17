#include "atm_sys.h"

void regMenu(char a[50], char pass[50])
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



// Function to generate random salt using the rand() function
void generate_salt(unsigned char salt[SALT_SIZE])
{
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < SALT_SIZE; i++)
    {
        salt[i] = rand() % 256; // Generate a random byte (0-255)
    }
}

// Simple SHA-256 implementation (basic one, replace with your own or other library if needed)
void simple_sha256(const unsigned char *input, size_t len, unsigned char output[HASH_SIZE])
{
    // Simplified version of a hash, replace with actual SHA-256 if you can
    for (size_t i = 0; i < len; i++)
    {
        output[i % HASH_SIZE] ^= input[i]; // Simple xor hashing as placeholder
    }
}

void hash_password(const char *password, const unsigned char *salt, unsigned char output[HASH_SIZE])
{
    unsigned char salted_password[SALT_SIZE + strlen(password)];

    // Combine salt and password
    memcpy(salted_password, salt, SALT_SIZE);
    memcpy(salted_password + SALT_SIZE, password, strlen(password));

    // Simple hash the salted password (replace with SHA-256 as needed)
    simple_sha256(salted_password, SALT_SIZE + strlen(password), output);
}

int compare_hashes(unsigned char hash1[HASH_SIZE], unsigned char hash2[HASH_SIZE]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        if (hash1[i] != hash2[i]) {
            return 0; // Passwords do not match
        }
    }
    return 1; // Passwords match
}

void registerUser(sqlite3 *db, const char *username, const char *password)
{
    unsigned char salt[SALT_SIZE];
    unsigned char hashed_password[HASH_SIZE];

    // Generate a random salt
    generate_salt(salt);
    // Hash the password with the salt
    hash_password(password, salt, hashed_password);

    // Insert into the database (assume db is a valid SQLite connection)
  //  char *sql = "INSERT INTO users (username, salt, passwd) VALUES (*username, salt, *hashed_password)";
   // Prepare SQL statement for safe insertion (prevent SQL injection)
    const char *sql = "INSERT INTO users (username, salt, passwd) VALUES (?, ?, ?)";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind parameters to the SQL query
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 2, salt, SALT_SIZE, SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 3, hashed_password, HASH_SIZE, SQLITE_STATIC);

    // Execute the SQL query
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Failed to insert user: %s\n", sqlite3_errmsg(db));
    } else {
        printf("User successfully registered!\n");
    }

    // Clean up
    sqlite3_finalize(stmt);
}


// Function to check if username already exists
int username_exists(sqlite3 *db, const char *username) {
    const char *sql = "SELECT COUNT(*) FROM users WHERE username = ?";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1; // Error
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    int exists = 0; // To hold the existence status
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0); // Get the count of matching rows
    }

    sqlite3_finalize(stmt); // Clean up
    return exists > 0; // Return true (1) if exists, otherwise false (0)
}
















// initializing
/*
void SetData(Data *sqlData)
{
    int resCode = sqlite3_open("data/DATA.db", &sqlData->db);
    if (resCode != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(sqlData->db));
        sqlite3_close(sqlData->db);
        return 1;
    }
}*/
// psudo funcs
//  TODO : add your **Update account information** function
void UpdateAccInfo(User u)
{
}

// TODO : add your **Make transaction** function
void MakeTransaction(User u)
{
}
// TODO : add your **Transfer owner** function
void TransferOwnership(User u)
{
}
// TODO : add your **Check the details of existing accounts** function
void CheckExistingaccs(User u)
{
}
// TODO : add your **Remove existing account** function
void RemoveAcc(User u)
{
}