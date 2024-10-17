#ifndef ATM_SYS_H
#define ATM_SYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "sqlite/sqlite3.h"
#include <time.h>

#define SALT_SIZE 16
#define HASH_SIZE 32 // SHA-256 digest size
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
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
void registerUser(sqlite3 *db, const char *username, const char *password);
int checkPassword(sqlite3 *db, User *usr);

// system function
void createNewAcc(sqlite3 *db, User u);
void mainMenu(sqlite3 *db, User u);
void checkAllAccounts(sqlite3 *db, User *usr);

// other
void exitErr(char *str);
void printOptions(int input);

// todo
void UpdateAccInfo(User u);

#endif