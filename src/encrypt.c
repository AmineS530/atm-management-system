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

// Simple SHA-256 implementation (basic one, replace with your own or other library if needed)
void XOR_hash(const unsigned char *input, size_t len, unsigned char output[HASH_SIZE])
{
    // Simplified version of a hash, replace with actual SHA-256 if you can
    for (size_t i = 0; i < len; i++)
    {
        output[i % HASH_SIZE] ^= input[i]; // Simple xor hashing as placeholder
    }
}

// Hash the password with the salt (you can replace this with a proper SHA-256 function)
void hash_password(const char *password, const unsigned char *salt, unsigned char output[HASH_SIZE])
{
    unsigned char salted_password[SALT_SIZE + strlen(password)];

    // Combine salt and password
    memcpy(salted_password, salt, SALT_SIZE);
    memcpy(salted_password + SALT_SIZE, password, strlen(password));

    // Simple hash the salted password (replace with SHA-256 as needed)
    XOR_hash(salted_password, SALT_SIZE + strlen(password), output);
}

// Function to print data in hexadecimal format
void print_hex(unsigned char data[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main()
{
    const char *password = "mySecretPassword123";
    unsigned char salt[SALT_SIZE];
    unsigned char hashed_password[HASH_SIZE];

    // Generate a random salt
    generate_salt(salt);

    // Hash the password with the salt
    hash_password(password, salt, hashed_password);

    // Print the salt and hashed password
    printf("Salt: ");
    print_hex(salt, SALT_SIZE);
    printf("Hashed Password: ");
    print_hex(hashed_password, HASH_SIZE);

    return 0;
}
