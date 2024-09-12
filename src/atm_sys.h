#ifndef ATM_SYS_H
#define ATM_SYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "sqlite/sqlite3.h"
//#include <openssl/sha.h>

#define HASH_SIZE SHA256_DIGEST_LENGTH

typedef struct s_date
{
    int month, day, year;
} Date;

// all fields for each record of an account
typedef struct s_record
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

// authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(User u);

// system function
void createNewAcc(User u);
void mainMenu(User u);
void checkAllAccounts(User u);

// other
void exitErr(char *str);
void printOptions(int input);
#endif