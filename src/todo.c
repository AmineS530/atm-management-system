#include "atm_sys.h"
// account types
// place holder : https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c

const char *accountTypes[5] = {"Current", "Savings", "Fixed01", "Fixed02", "Fixed03"};

//     printf("\nEnter today's date(mm/dd/yyyy):");
//     scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

// void get_date(char *deposit_date)
// {
//     struct tm set_time = {0};

//     system("clear");
//     printf("Enter the date!\n");

// bad_day:
//     printf("Day : ");
//     scanf("%d", &set_time.tm_mday);
//     if (set_time.tm_mday < 1 || set_time.tm_mday > 31)
//     {
//         printf("Invalid day! Please enter a value between 1 and 31.\n");
//         goto bad_day;
//     }
// bad_month:
//     printf("Month: ");
//     scanf("%d", &set_time.tm_mon);
//     if (set_time.tm_mon < 1 || set_time.tm_mon > 12)
//     {
//         printf("Invalid month! Please enter a value between 1 and 12.\n");
//         goto bad_month;
//     }
//     set_time.tm_mon -= 1;
// bad_year:
//     printf("Year: ");
//     scanf("%d", &set_time.tm_year);
//     if (set_time.tm_year < 1980 || set_time.tm_year > 2025)
//     {
//         printf("Invalid year! Please enter a value between 1980 and 2025.\n");
//         goto bad_year;
//     }
//     set_time.tm_year -= 1900;

//     strftime(deposit_date, sizeof(deposit_date), "%a %b %d %H:%M:%S %Y", &set_time);
//     printf("Entered Date: %s\n", deposit_date);
// }

Record fillInfo(sqlite3 *db, User usr)
{
    Record info;
    int err;

    info.userId = usr.id;
    //get_date(info.deposit);

    get_account_nbr(&info, db);

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
    return info;
}

void get_full_name(Record *info)
{
invalid:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter your full name: ");
    if (!safeInput(info->name) && strlen(info->name) > 0 && strlen(info->name) < MAX_STR_LEN)
        goto invalid;
}

void clear_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int safeInput(char *buffer) {
    clear_buffer();
    if (scanf(STRING_TO_SCAN, buffer) == 1) {  
        return 1;
    }
    buffer[0] = '\0';
    return 0;
}

void get_account_nbr(Record *info, sqlite3 *db)
{
    char input[20];
    long accNbr;
    char *endptr;
    int errno;
invalid:
    clear_buffer();
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter account number: ");

    if (scanf("%20s", input) != 1)
        goto invalid;
    // Ensure input is all digits
    for (char *c = input; *c; c++)
        if (!is_digit(*c))
            goto invalid;
    errno = 0;
    accNbr = strtol(input, &endptr, 10);
    if (*endptr || errno == ERANGE || accNbr < 0) {
        printf("Invalid account number!\n");
        sleep(2);
        goto invalid;
    }
    if (account_exists(accNbr, db)){
        printf("\tAccount number already exists\n\n");
        goto invalid;
    }
    info->accountNbr = accNbr;
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
    clear_buffer();
}

void get_balance(Record *info)
{
    int errno;
    char input[20];
    char *endptr;
    double balance;
invalid:
    errno = 0;
    clear_buffer();
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("\tEnter balance: ");
    if (scanf("%s", input) != 1)
    {
        printf("✖ Invalid input! Please enter a valid balance.\n");
        sleep(2);
        goto invalid;
    } 
    balance = strtol(input, &endptr, 10);
     if (*endptr || errno == ERANGE || balance < 0) {
        printf("✖ Invalid input! Please enter a valid balance.\n");
        sleep(2);
        goto invalid;
    }
    info->balance = balance;
}

// //TODO : **Create new account** function
void createNewAcc(sqlite3 *db, User usr)
{
    if (usr.accCount >= MAX_ACCOUNTS)
    {
        printf("You have reached the maximum number of accounts you can create.\n");
        sleep_sec(3);
        return;
    }

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
    printf("Account number:jkhjkhjk");
    // Bind values to the prepared statement
    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int64(stmt, 2, info.accountNbr);
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
void UpdateAccInfo(User usr, sqlite3 *db)
{
    char input[30];
    sqlite3_stmt *stmt;
    int prompt, choice;
    int err = 0;
    char sql[MAX_STR_LEN], sqlold[MAX_STR_LEN];
    const char *query = NULL;
    char old_val[MAX_STR_LEN];
    memset(old_val, 0, MAX_STR_LEN);
    system("clear");

    for (int i = 0; i < usr.accCount && usr.accountIds[i]; i++)
        printf("[%d] Account number: %ld\n", i, usr.accountIds[i]);

    while (1)
    {
        choice = -1;
        printf("Enter account number: ");
        if (scanf("%d", &choice) != 1)
            printf("✖ Invalid input! Please enter a valid number.\n");
        else if (choice < 0 || choice > (usr.accCount - 1))
        {
            printf("✖ Invalid option! Please enter a number between 0 and %d.\n", usr.accCount - 1);
        }
        else
            break;
    }
    system("clear");
invalid:
    prompt = -1;
    clear_buffer();
    printf("\t\t====== Update Account Informations =====\n\n");
    printf("\t\tselected account number: %ld\n", usr.accountIds[choice]);
    printf("\t\tOptions:\n\n\t\t"
           "[1] Update Country\n\t\t"
           "[2] Update Phone-Number\n\n\t\t"
           "[3] Exit\n\n\t\t"
           "Your input: ");
    scanf("%d", &prompt);
    if (prompt == 1)
    {
        query = "country";
        do
        {
            err = check_country(input);
            system("clear");
            printf("\t\t\t===== New record =====\n");
        } while (err != 1);
    }
    else if (prompt == 2)
    {
        query = "phone";
        do
        {
            err = check_phone_numb(input);
            system("clear");
            printf("\t\t\t===== New record =====\n");
        } while (err != 1);
    }
    else if (prompt == 3)
        mainMenu(db, usr);
    else
    {
        system("clear");
        // can store this in a var that start empty so its appear under update acc info txt or re add text here
        printf("\n   | [+] Non-Valid input |\n\n");
        goto invalid;
    }
    snprintf(sqlold, sizeof(sqlold), "SELECT %s FROM records WHERE accNbr = ?", query);
    if (sqlite3_prepare_v2(db, sqlold, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_int64(stmt, 1, usr.accountIds[choice]);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const unsigned char *old = sqlite3_column_text(stmt, 0);
        strncpy(old_val, (const char *)old, sizeof(old_val) - 1);
    }
    sqlite3_finalize(stmt);
    snprintf(sql, sizeof(sql), "UPDATE records SET %s = ? WHERE accNbr = ?", query);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 2, usr.accountIds[choice]);
    if (sqlite3_step(stmt) != SQLITE_DONE || err == 0)
    {
        system("clear");
        fprintf(stderr, "\t\tExecution failed\n");
    }
    sqlite3_finalize(stmt);
    printf("Account information updated successfully!\n"
           "old value: %s\n"
           "new value: %s\n",
           old_val, input);
    sleep_sec(3);
}

// TODO : add your **Make transaction** function
// void MakeTransaction(User *usr, sqlite3 *db)
// {
// }

// TODO : add your **Transfer owner** function
// void TransferOwnership(User *usr, sqlite3 *db)
// {
// }

// TODO : add your **Check the details of existing accounts** function
void CheckExistingaccs(User usr, sqlite3 *db)
{
    const char *sql = "SELECT accNbr, created_at, country, phone, balance, accType "
                      "FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    int choice;

    for (int i = 0; i < usr.accCount && usr.accountIds[i]; i++)
        printf("[%d] Account number: %ld\n", i, usr.accountIds[i]);

    while (1)
    {
        choice = -1;
        printf("Enter account number: ");
        if (scanf("%d", &choice) != 1)
            printf("✖ Invalid input! Please enter a valid number.\n");
        else if (choice < 0 || choice > (usr.accCount - 1))
        {
            printf("✖ Invalid option! Please enter a number between 0 and %d.\n", usr.accCount - 1);
        }
        else
            break;
    }
    system("clear");
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int64(stmt, 2, usr.accountIds[choice]);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        long accNbr = sqlite3_column_int64(stmt, 0);
        const unsigned char *created_at = sqlite3_column_text(stmt, 1);
        const unsigned char *country = sqlite3_column_text(stmt, 2);
        const unsigned char *phone = sqlite3_column_text(stmt, 3);
        double balance = sqlite3_column_double(stmt, 4);
        const unsigned char *accountType = sqlite3_column_text(stmt, 5);

        printf("_____________________\n");
        printf("Account number: %ld\n", accNbr);
        printf("Created Date: %s\n", created_at);
        printf("Country: %s\n", country);
        printf("Phone: %s\n", phone);
        printf("Balance: %.2f\n", balance);
        printf("Account Type: %s\n", accountType);
        printf("_____________________\n");
        caculateInterest(accountType, balance, accNbr);
        // make print info func and fill n print here and print all recs
    }
    sqlite3_finalize(stmt);
}

void caculateInterest(const unsigned char *accountType, double balance, long account_nbr)
{
    if (strcmp((const char *)accountType, "Current") == 0)
    {
        printf("You will not get interests because the account is of type current.\n");
    }
    else
    {
        double interestRate = 0.0;
        if (strcmp((const char *)accountType, "Savings") == 0)
        {
            interestRate = 0.07;
        }
        else if (strcmp((const char *)accountType, "Fixed01") == 0)
        {
            interestRate = 0.04;
        }
        else if (strcmp((const char *)accountType, "Fixed02") == 0)
        {
            interestRate = 0.05;
        }
        else if (strcmp((const char *)accountType, "Fixed03") == 0)
        {
            interestRate = 0.08;
        }
        /*For example: for an account of type savings with a deposit date of 10/10/2002 and an amount of $1023.20 the system will show
         "You will get $5.97 as interest on day 10 of every month".*/
        double interest = balance * interestRate;
        printf("Interest for account number %ld is: $%.2f\n", account_nbr, interest);
    }
}

// TODO : add your **Remove existing account** function
// void RemoveAcc(User *usr, sqlite3 *db)
// {
// }

void getAccNbrs(User *usr, sqlite3 *db)
{
    sqlite3_stmt *stmt;
    usr->accCount = 0;
    if (usr == NULL)
    {
        printf("Error: User pointer is NULL.\n");
        return;
    }

    // get account ids
    const char *sql = "SELECT accNbr FROM records WHERE userID = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, usr->id);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        long accNbr = sqlite3_column_int64(stmt, 0);
        usr->accountIds[usr->accCount] = accNbr;
        usr->accCount++;
    }
    sqlite3_finalize(stmt);
}

// Function to check if username already exists
int account_exists(long accNbr, sqlite3 *db)
{
    const char *sql = "SELECT COUNT(*) FROM records WHERE accNbr = ?";
    sqlite3_stmt *stmt;
    int exists = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int64(stmt, 1, accNbr);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        exists = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return exists;
}