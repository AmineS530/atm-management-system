#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to sanitize input by escaping special characters
char* sanitize_input(const char *input) {
    size_t len = strlen(input);
    // Allocate memory for worst-case scenario where every character needs escaping
    char *sanitized = malloc(len * 2 + 1);  // Worst case: every character needs to be escaped
    if (!sanitized) {
        return NULL;  // Memory allocation failed
    }

    size_t j = 0;  // Index for sanitized string
    for (size_t i = 0; i < len; i++) {
        // Escape special SQL characters
        if (input[i] == '\'' || input[i] == '\"' || input[i] == '\\') {
            sanitized[j++] = '\\';  // Add escape character before the special char
        }
        sanitized[j++] = input[i];
    }
    sanitized[j] = '\0';  // Null-terminate the string

    return sanitized;
}

int main() {
    const char *unsafe_input = "Robert'; DROP TABLE users;--";
    char *safe_input = sanitize_input(unsafe_input);

    if (safe_input) {
        printf("Sanitized input: %s\n", safe_input);
        free(safe_input);  // Free the allocated memory after use
    } else {
        printf("Memory allocation failed!\n");
    }

    return 0;
}
/*
// Example of using prepared statements in SQLite (no need for manual sanitization)
const char *sql = "SELECT * FROM users WHERE username = ?";
sqlite3_stmt *stmt;

if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
    // Bind the user-provided input safely
    sqlite3_bind_text(stmt, 1, user_input, -1, SQLITE_STATIC);
    // Now execute the query without worry of SQL injection
}
*/
