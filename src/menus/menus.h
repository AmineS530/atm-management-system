#ifndef MENUS_H
#define MENUS_H

#include "../helpers/helpers.h"
#include "../auth/auth.h"
#include <math.h>

/* menus.c*/
void login_menu(User *usr);
int register_menu(sqlite3 *db, User *usr);
void main_menu(sqlite3 *db, User usr);
int init_menu(sqlite3 *db, User *usr);
void just_a_menu();


void create_new_acc(User *usr, sqlite3 *db);

void update_acc_info(User usr, sqlite3 *db);

void check_existing_accs(User usr, sqlite3 *db);

void check_all_accounts(User usr, sqlite3 *db);

void make_transaction(sqlite3 *db, User usr);

void remove_acc(sqlite3 *db, User *usr);

void transfer_ownership(sqlite3 *db, User *usr);


#endif