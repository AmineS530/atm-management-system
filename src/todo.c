#include "atm_sys.h"

// static const char *RECORDS = "./data/records.txt";

// //TODO : **Create new account** function
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

int fillInfo(sqlite3 *db, User usr)
{
    Record info;
    int err;

    info.userId = usr.id;
    get_full_name(&info);
    err = check_phone_numb(info.phone);
    if (err == 0)
        return 0;
    err = check_country(info.country);
    if (err == 0)
        return 0;
    // info.accountType = get_account_type(info);
    // info.balance = get_balance(info);
    getdate(&info.deposit);
    return 1;
}

void get_full_name(Record *info)
{
    invalid:
    printf("Enter your full name: ");
    if (!safeInput(info->name, MAX_STR_LEN)){
       goto invalid;
    }
}


int safeInput(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) != NULL) {
        // Remove trailing newline, if any
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // Clear the remaining buffer if input exceeds size
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        return 1; // Input successful
    } else {
        buffer[0] = '\0'; // Clear buffer in case of error
        return 0; // Input failed
    }
}
void get_name(Record *info) {};
void get_balance(Record *info) {};
void get_account_type(Record *info) {};

void createNewAcc(sqlite3 *db, User u)
{
    const char *sql = "INSERT INTO records (userID, username, accNbr, name ,country, phone, balance, accType, balance) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
}

//  TODO :  **Update account information** function
void UpdateAccInfo(User *usr, sqlite3 *db)
{
    char input[30];
    sqlite3_stmt *stmt;
    int prompt;
    int err;
    const char *sql = NULL;
    system("clear");
invalid:
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
        system("clear");
        // can store this in a var that start empty so its appear under update acc info txt or re add text here
        printf("\n   | [+] Non-Valid input |\n\n");
        goto invalid;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, usr->name, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE || err == 0)
    {
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