#include "atm_sys.h"


// int getAccountFromFile(FILE *ptr, char name[50], Record *r)
// {
//     return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %s %lf %s",
//                   &r->id,
//                   &r->userId,
//                   name,
//                   &r->accountNbr,
//                   &r->deposit.month,
//                   &r->deposit.day,
//                   &r->deposit.year,
//                   r->country,
//                   &r->phone,
//                   &r->balance,
//                   r->accountType) != EOF;
// }

// void saveAccountToFile(FILE *ptr, User u, Record r)
// {
//     fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
//             r.id,
//             u.id,
//             u.name,
//             r.accountNbr,
//             r.deposit.month,
//             r.deposit.day,
//             r.deposit.year,
//             r.country,
//             r.phone,
//             r.balance,
//             r.accountType);
// }

void stayOrReturn(int notGood, void f(User u), User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(NULL, u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
        mainMenu(NULL, u);
    else
    {
        system("clear");
        exit(1);
    }
}

void success(User usr)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");

    if (option == 1)
        mainMenu(NULL, usr);
    else if (option == 0)
        exit(1);
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}


// check all accounts for a user
void checkAllAccounts(sqlite3 *db, User *usr)
{
    const char *sql = "SELECT accNbr, created_at, country, phone, balance, accType FROM records WHERE userID = ?";
    sqlite3_stmt *stmt;

    system("clear");
    printf("\t\t====== All accounts for user: %s =====\n\n", usr->name);

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, usr->id);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int accNbr = sqlite3_column_int(stmt, 0);
        const unsigned char *created_at = sqlite3_column_text(stmt, 1);
        const unsigned char *country = sqlite3_column_text(stmt, 2);
        const unsigned char *phone = sqlite3_column_text(stmt, 3);
        float balance = sqlite3_column_double(stmt, 4);
        const unsigned char *accountType = sqlite3_column_text(stmt, 5);

        printf("_____________________\n");
        printf("Account number: %d\n", accNbr);
        printf("Created Date: %s\n", created_at);
        printf("Country: %s\n", country);
        printf("Phone number: %s\n", phone);
        printf("Balance: $%.2f\n", balance);
        printf("Type Of Account: %s\n", accountType);
        printf("\n");
    }

    sqlite3_finalize(stmt);
}
