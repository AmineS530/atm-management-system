#ifndef ATM_SYS_H
#define ATM_SYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <limits.h>

#include "sqlite/sqlite3.h"

#define DB_PATH "data/DATA.db"
#define SALT_SIZE 16
#define HASH_SIZE 32
#define STRING_TO_SCAN "%49s"
#define MAX_STR_LEN 50
#define MAX_ACCOUNTS 8
#define MAX_ATTEMPTS 3

// all fields for each record of an account
typedef struct s_records
{
    int id;
    int userId;
    char name[50];
    char country[30];
    char phone[20];
    char *accountType;
    long accountNbr;
    double balance;
    char *deposit;
    struct tm *withdraw;
} Record;

typedef struct s_User
{
    int id;
    char name[50];
    char password[50];
    long accountIds[MAX_ACCOUNTS];
    int accCount;
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
void registerMenu(sqlite3 *db, User *usr);

// system function
int registerUser(sqlite3 *db, User *usr);
void createNewAcc(sqlite3 *db, User u);
void checkAllAccounts(sqlite3 *db, User *usr);

// other
void exitErr(char *str);
void printOptions(int input, char *name);
int username_exists(sqlite3 *db, User usr);
void success(sqlite3 *db, User usr);
void stayOrReturn( sqlite3 *db ,int notGood, void f(User u), User u);
void sleep_sec(int seconds);
void just_a_menu();

// todo
void UpdateAccInfo(User usr, sqlite3 *db);
void get_account_type(Record *info);
void get_full_name(Record *info);
void get_balance(Record *info);
void get_account_nbr(Record *info);
void getAccNbrs(User *usr, sqlite3 *db);

//input utils 
int check_phone_numb(char *phone);
int check_country(char *country);
char *to_upper(char *str);
int safeInput(char *buffer, size_t size);
void clear_buffer(void);

#endif