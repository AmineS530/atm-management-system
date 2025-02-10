#ifndef ATM_SYS_H
#define ATM_SYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include "sqlite/sqlite3.h"

#define DB_PATH "/home/amines/Code/zone01/C-Lang/atm-management-system/data/DATA.db"
#define SALT_SIZE 16
#define HASH_SIZE 32
#define STRING_TO_SCAN "%49s"
#define MAX_STR_LEN 50
#define MAX_ACCOUNTS 8
#define MAX_ATTEMPTS 3

// all fields for each record of an account
typedef struct s_records
{
    double balance;
    long accountNbr;
    int id;
    int userId;
    char name[50];
    char *country;
    char *phone;
    char *accountType;
    char *deposit;
    struct tm *withdraw;
} Record;

typedef struct s_User
{
    long accountIds[MAX_ACCOUNTS];
    int id;
    int accCount;
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
void registerMenu(sqlite3 *db, User *usr);
void CheckExistingaccs(User usr, sqlite3 *db);

// system function
int registerUser(sqlite3 *db, User *usr);
void createNewAcc(User usr, sqlite3 *db);
void checkAllAccounts(User usr, sqlite3 *db);

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
void get_account_nbr(Record *info, sqlite3 *db);
void getAccNbrs(User *usr, sqlite3 *db);
int account_exists(long accNbr, sqlite3 *db);
void caculateInterest(const unsigned char *accountType, double balance, long account_nbr);
void printAccounts(Record rec);


//input utils 
int check_phone_numb(char *phone);
int check_country(char *country);
char *to_upper(char *str);
int is_digit(int c);
int safeInput(char *buffer);
void clear_buffer(void);

#endif