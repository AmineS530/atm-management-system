#include "atm_sys.h"

// static const char *RECORDS = "./data/records.txt";

// TODO : **Create new account** function
// void createNewAcc(sqlite3 *db, User u)
// {
//     Record r;
//     Record cr;
//     char userName[50];
//     FILE *pf = fopen(RECORDS, "a+");

// noAccount:
//     system("clear");
//     printf("\t\t\t===== New record =====\n");

//     printf("\nEnter today's date(mm/dd/yyyy):");
//     scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
//     printf("\nEnter the account number:");
//     scanf("%d", &r.accountNbr);

//     while (getAccountFromFile(pf, userName, &cr))
//     {
//         if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
//         {
//             printf("✖ This Account already exists for this user\n\n");
//             goto noAccount;
//         }
//     }
//     printf("\nEnter the country:");
//     scanf("%s", r.country);
//     printf("\nEnter the phone number:");
//     scanf("%d", &r.phone);
//     printf("\nEnter amount to deposit: $");
//     scanf("%lf", &r.balance);
//     printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
//     scanf("%s", r.accountType);

//     saveAccountToFile(pf, u, r);

//     fclose(pf);
//     success(u);
// }

//  TODO :  **Update account information** function
void UpdateAccInfo(User *usr, sqlite3 *db)
{
    char input[30];
    sqlite3_stmt *stmt;
    int prompt;
    int err;
    const char *sql = NULL;

invalid:
    system("clear");
    printf("\t\t====== Update Account Informations =====\n\n");
    printf("\t\tOptions:\n\n\t\t[1] Update Country\n\t\t[2] Update Phone-Number\n\n\t\tYour input: ");
    scanf("%d", &prompt);
    if (prompt == 1)
    {
        sql = "UPDATE records SET country = ? WHERE username = ?";
        err = check_country(input);
    }
    else if (prompt == 2)
    {
        sql = "UPDATE records SET phone = ? WHERE username = ?";
        err = check_phone_numb(input);
    }
    else
    {
        return;
        goto invalid;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, usr->name, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE ||  err == 0){
        system("clear");
        fprintf(stderr, "\t\tExecution failed\n");
    }

    sqlite3_finalize(stmt);
}

// TODO : add your **Make transaction** function
void MakeTransaction(User *usr, sqlite3 *db)
{
}

// TODO : add your **Transfer owner** function
void TransferOwnership(User *usr, sqlite3 *db)
{
}

// TODO : add your **Check the details of existing accounts** function
void CheckExistingaccs(User u, sqlite3 *db)
{
}

// TODO : add your **Remove existing account** function
void RemoveAcc(User *usr, sqlite3 *db)
{
}