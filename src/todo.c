#include "atm_sys.h"
// account types
// place holder : https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c

const char *accountTypes[5] = {"Current", "Savings", "Fixed01", "Fixed02", "Fixed03"};

// //TODO : **Create new account** function
// void createNewAcc(sqlite3 *db, User u)
// {
//     Record r;
//     Record cr;
//     char userName[50];
//     FILE *pf = fopen(RECORDS, "a+");

// noAccount:
//     system("clear");
//

//     printf("\nEnter today's date(mm/dd/yyyy):");
//     scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
//     printf("\nEnter the account number:");
//     scanf("%d", &r.accountNbr);

Record fillInfo(sqlite3 *db, User usr)
{
    Record info;
    int err;
    time_t timestamp;

    info.userId = usr.id;
    get_account_nbr(&info);
    get_full_name(&info);
checkphone:
    printf("\t\t\t===== New record =====\n");
    err = check_phone_numb(info.phone);
    if (err == 0)
    {
        system("clear");
        printf("Phone number is not valid!\n");
        goto checkphone;
    }
    system("clear");
checkcountry:
    printf("\t\t\t===== New record =====\n");
    err = check_country(info.country);
    if (err == 0)
    {
        system("clear");
        printf("Country is not valid!\n");
        goto checkcountry;
    }
    get_account_type(&info);
    get_balance(&info);
    timestamp = time(NULL);
    info.deposit = asctime(gmtime(&timestamp));
    return info;
}

void get_full_name(Record *info)
{
invalid:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter your full name: ");
    if (!safeInput(info->name, MAX_STR_LEN) && strlen(info->name) > 0 && strlen(info->name) < MAX_STR_LEN)
        goto invalid;
}

int safeInput(char *buffer, size_t size)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    if (fgets(buffer, size, stdin) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        return 1;
    }
    else
    {
        buffer[0] = '\0'; // Clear buffer in case of error
        return 0;
    }
}
void get_account_nbr(Record *info)
{
invalid:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter account number: ");
    if (scanf("%d", &info->accountNbr) != 1)
        printf("✖ Invalid input! Please enter a valid number.\n");
    else if (info->accountNbr < 0 || info->accountNbr > MAX_ACCOUNTS)
        printf("✖ Invalid account number! Please enter a number between 0 and %d.\n", MAX_ACCOUNTS);
    else
        goto invalid;
}

void get_account_type(Record *info)
{
    int input = 0;
invalid:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter account type:"
           "\n\t\t[1] current"
           "\n\t\t[2] savings: interest rate 7%%"
           "\n\t\t[3] fixed01(1 year account): interest rate 4%%"
           "\n\t\t[4] fixed02(2 year account): interest rate 5%%"
           "\n\t\t[5] fixed03(3 year account): interest rate 8%%\n");
    scanf("%d", &input);
    if (input < 1 || input > 5)
    {
        system("clear");
        printf("Invalid account type!\n");
        goto invalid;
    }
    info->accountType = (char *)accountTypes[input - 1];
};

void get_balance(Record *info)
{
invalid:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter balance: ");
    if (scanf("%le", &info->balance) != 1 || info->balance < 0)
    {
        printf("✖ Invalid input! Please enter a valid balance.\n");
        sleep_sec(3);
        goto invalid;
    }
}

void createNewAcc(sqlite3 *db, User usr)
{
    // SQL query to insert data into the `records` table
    const char *sql = "INSERT INTO records (userID, accNbr, fullname, country, phone, balance, accType) VALUES (?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    Record info;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Call a helper function to fill the Record structure
    info = fillInfo(db, usr);

    // Bind values to the prepared statement
    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int(stmt, 2, info.accountNbr);
    sqlite3_bind_text(stmt, 3, info.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, info.country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, info.phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, info.balance);
    sqlite3_bind_text(stmt, 7, info.accountType, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
        printf("Error executing statement: %s\n", sqlite3_errmsg(db));
    else
        printf("New account created successfully!\n");

    sqlite3_finalize(stmt);
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
    printf("\t\tOptions:\n\n\t\t"
            "[1] Update Country\n\t\t"
            "[2] Update Phone-Number\n\n\t\t"
            "[3] Exit\n\n\t\t"
            "Your input: ");
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
    else if (prompt == 3)
        mainMenu(db, *usr);
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