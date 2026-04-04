#ifndef HELPERS_H
#define HELPERS_H

#include "../sqlite/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include "../input/input.h"

#define SALT_SIZE 16
#define HASH_SIZE 32
#define MAX_STR_LEN 50
#define MSG_LEN 128
#define MAX_USERS 64
#define MAX_PENDING 4
#define MAX_ACCOUNTS 8
#define MAX_ATTEMPTS 3
#define false 0
#define true 1

#define DB_PATH "./data/DATA.db"
#define SCHEMA_PATH "./data/schema.sql"

enum
{
    INIT_MENU_EXIT = 0,
    INIT_MENU_GO
};

enum
{
    OFFLINE = 0,
    ONLINE
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

// other
void main_menu(sqlite3 *db, User u);

/* init.c */
sqlite3 *init();
User init_user(void);

/* query_helpers.c */
void get_acc_nbrs(User *usr, sqlite3 *db);
int account_exists(long accNbr, sqlite3 *db);
float get_balance(sqlite3 *db, User usr, int choice);
int username_exists(sqlite3 *db, User usr);

/* utils.c */
//todo: to be removed
void print_options(int input, char *name);
void print_accounts(Record rec);
char *to_upper(char *str);
int select_account(User usr);
void sleep_sec(int seconds);

/* account_logging.c */
char *format_account(Record rec);
Record get_record_by_accNbr(sqlite3 *db, long accNbr);

/* system.c */
void stay_or_return(sqlite3 *db, int notGood, void f(sqlite3 *db, User u), User u);
void success();

/* logs.c */
void log_info(const char *user, const char *fmt, ...);
void log_error(const char *user, const char *fmt, ...);

/* errors.c */
void exit_err(char *str);

#endif