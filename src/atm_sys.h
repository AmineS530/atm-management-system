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

#define DB_PATH "./data/DATA.db"
#define SCHEMA_PATH "./data/schema.sql"

#define SALT_SIZE 16
#define HASH_SIZE 32
#define STRING_TO_SCAN "%49s"
#define MAX_STR_LEN 50
#define MAX_ACCOUNTS 8
#define MAX_ATTEMPTS 3
#define false 0
#define true 1

enum
{
    INIT_MENU_EXIT = 0,
    INIT_MENU_GO
};

// all fields for each record of an account
typedef struct s_records
{
    double balance;
    long accountNbr;
    int id;
    int userId;
    char name[50];
    char country[30];
    char phone[20];
    char accountType[20];
    char deposit[30];
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

// authentication functions
int check_password(sqlite3 *db, User *usr);
void hash_password(char *password, const unsigned char *salt, unsigned char output[HASH_SIZE]);
User init_user(void);

// menus
int init_menu(sqlite3 *db, User *usr);
void main_menu(sqlite3 *db, User u);
void login_menu(User *usr);
int register_menu(sqlite3 *db, User *usr);
void check_existing_accs(User usr, sqlite3 *db);

// system function
int register_user(sqlite3 *db, User *usr);
void create_new_acc(User usr, sqlite3 *db);
void check_all_accounts(User usr, sqlite3 *db);
void make_transaction(sqlite3 *db, User usr);
// other
void exit_err(char *str);
void print_options(int input, char *name);
int username_exists(sqlite3 *db, User usr);
void success(sqlite3 *db, User usr);
void stay_or_return(sqlite3 *db, int notGood, void f(User u), User u);
void sleep_sec(int seconds);
void just_a_menu();
void get_date(char *deposit_date);

// todo
void update_acc_info(User usr, sqlite3 *db);
void get_account_type(Record *info);
void get_full_name(Record *info);
void insert_balance(Record *info);
void get_account_nbr(Record *info, sqlite3 *db);
void get_acc_nbrs(User *usr, sqlite3 *db);
int account_exists(long accNbr, sqlite3 *db);
void caculate_interest(const unsigned char *accountType, double balance, long account_nbr);
void print_accounts(Record rec);
int get_acc_type(sqlite3 *db, User usr, int choice, char buffer[8]);
int withdraw(sqlite3 *db, User usr, int choice, float balance);
int deposit(sqlite3 *db, User usr, int choice, float balance);
float get_balance(sqlite3 *db, User usr, int choice);
Record fill_info(sqlite3 *db, User usr);
sqlite3 *init();

// input utils
int check_phone_numb(char *phone);
int check_country(char *country);
char *to_upper(char *str);
int is_alpha(int c);
int is_digit(int c);
int safe_string_input(char *buffer, size_t size);
int safe_int_input(int *value);
int safe_float_input(float *value);
#endif