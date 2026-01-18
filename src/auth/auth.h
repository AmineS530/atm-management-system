#ifndef AUTH_H
#define AUTH_H

#include "../helpers/helpers.h"

/* auth.c */
int check_password(sqlite3 *db, User *usr);

/* encrypt.c */
void hash_password(char *password, const unsigned char *salt, unsigned char output[HASH_SIZE]);
int register_user(sqlite3 *db, User *usr);


#endif