#ifndef ATM_SYS_H
#define ATM_SYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include "sqlite/sqlite3.h"

#define DB_PATH "data/DATA.db"
#define SALT_SIZE 16
#define HASH_SIZE 32
typedef struct s_date
{
    int month, day, year;
} Date;

// all fields for each record of an account
typedef struct s_records
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double balance;
    Date deposit;
    Date withdraw;
} Record;

typedef struct s_User
{
    int id;
    char name[50];
    char password[50];
} User;

typedef struct s_data
{
    sqlite3 *db;
    Record records;
    User users;
} Data;

// authentication functions
int checkPassword(sqlite3 *db, User *usr);
void hash_password(const char *password, const unsigned char *salt, unsigned char output[HASH_SIZE]);
int compare_hashes(unsigned char hash1[HASH_SIZE], unsigned char hash2[HASH_SIZE]);

// menus
void initMenu(sqlite3 *db, User *usr);
void mainMenu(sqlite3 *db, User u);
void loginMenu(User *usr);
void registerMenu(User *usr, char pass[50]);

// system function
void registerUser(sqlite3 *db, User *usr);
void createNewAcc(sqlite3 *db, User u);
void checkAllAccounts(sqlite3 *db, User *usr);

// other
void exitErr(char *str);
void printOptions(int input);
int username_exists(sqlite3 *db, User usr);

// todo
void UpdateAccInfo(User u);

#endif