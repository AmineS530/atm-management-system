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
#define STRING_TO_SCAN "%49s"
#define MAX_STR_LEN 50

// account types
// place holder : https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c
const char *accountType[5] = {"current", "savings", "fixed01", "fixed02", "fixed03"};

// all fields for each record of an account
typedef struct s_records
{
    int id;
    int userId;
    char name[50];
    char country[30];
    char phone[20];
    char accountType[10];
    int accountNbr;
    double balance;
    struct tm *deposit;
    struct tm *withdraw;
} Record;

typedef struct s_User
{
    int id;
    char name[50];
    char password[50];
} User;

// typedef struct s_data
// {
//     sqlite3 *db;
//     Record records;
//     User users;
// } Data;

// authentication functions
int checkPassword(sqlite3 *db, User *usr);
void hash_password(char *password, const unsigned char *salt, unsigned char output[HASH_SIZE]);

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
void printOptions(int input, char *name);
int username_exists(sqlite3 *db, User usr);

// todo
void UpdateAccInfo(User *usr, sqlite3 *db);
void get_account_type(Record *info);
void get_full_name(Record *info);
int safeInput(char *buffer, size_t size);



//input utils 
int check_phone_numb(char *phone);
int check_country(char *country);
char *to_upper(char *str);

#endif