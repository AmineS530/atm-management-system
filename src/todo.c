#include "atm_sys.h"
// account types
// place holder : https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c

const char *accountTypes[5] = {"Current", "Savings", "Fixed01", "Fixed02", "Fixed03"};

//     printf("\nEnter today's date(mm/dd/yyyy):");
//     scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

void get_date(char *deposit_date)
{
    struct tm set_time = {0};

    system("clear");
    printf("Enter the date!\n");

    // Day
    while (1)
    {
        printf("Day: ");
        if (safe_int_input(&set_time.tm_mday) != 1)
        {
            printf("Invalid input! Please enter a number.\n");

            continue;
        }
        if (set_time.tm_mday < 1 || set_time.tm_mday > 31)
        {
            printf("Invalid day! Please enter a value between 1 and 31.\n");
            continue;
        }
        break;
    }

    // Month
    while (1)
    {
        printf("Month: ");
        if (safe_int_input(&set_time.tm_mon) != 1)
        {
            printf("Invalid input! Please enter a number.\n");

            continue;
        }
        if (set_time.tm_mon < 1 || set_time.tm_mon > 12)
        {
            printf("Invalid month! Please enter a value between 1 and 12.\n");
            continue;
        }
        set_time.tm_mon -= 1; // tm_mon is 0-11
        break;
    }

    // Year
    while (1)
    {
        printf("Year: ");
        if (safe_int_input(&set_time.tm_year) != 1)
        {
            printf("Invalid input! Please enter a number.\n");

            continue;
        }
        if (set_time.tm_year < 1980 || set_time.tm_year > 2025)
        {
            printf("Invalid year! Please enter a value between 1980 and 2025.\n");
            continue;
        }
        set_time.tm_year -= 1900; // tm_year is years since 1900
        break;
    }

    // Format date into deposit_date
    strftime(deposit_date, 30, "%a %b %d %H:%M:%S %Y", &set_time);
    printf("Entered Date: %s\n", deposit_date);
    sleep(2);
}

Record fill_info(sqlite3 *db, User usr)
{
    Record info = {0};
    // int err;

    info.userId = usr.id;
    get_date(info.deposit);

    get_account_nbr(&info, db);
    get_full_name(&info);
    while (1)
    {
        system("clear");
        printf("\t\t\t===== New record =====\n");
        if (check_phone_numb(info.phone))
            break;
        system("clear");
        printf("Phone number is not valid!\n");
        sleep(2);
    }
    while (1)
    {
        system("clear");
        printf("\t\t\t===== New record =====\n");
        if (check_country(info.country))
            break;
        system("clear");
        printf("Country is not valid!\n");
        sleep(2);
    }
    get_account_type(&info);
    insert_balance(&info);
    return info;
}

void get_full_name(Record *info)
{
invalid:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter your full name: ");
    if (!safe_string_input(info->name, 0) && strlen(info->name) > 0 && strlen(info->name) < MAX_STR_LEN)
    {
        printf("✖ Invalid input! Please enter a valid name.\n");
        sleep(2);
        goto invalid;
    }
    for (size_t i = 0; i < strlen(info->name); i++)
        if (!(is_alpha(info->name[i]) || info->name[i] == ' '))
        {
            printf("✖ Invalid input! Please enter a valid name.\n");
            sleep(2);
            goto invalid;
        }
}

void get_account_nbr(Record *info, sqlite3 *db)
{
    char input[20];
    long accNbr;
    char *endptr;
    int errno;
invalid:

    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("Enter account number: ");

    if (safe_string_input(input, 20) != 1)
        goto invalid;
    // Ensure input is all digits
    for (char *c = input; *c; c++)
        if (!is_digit(*c))
            goto invalid;
    errno = 0;
    accNbr = strtol(input, &endptr, 10);
    if (*endptr || errno == ERANGE || accNbr < 0)
    {
        printf("Invalid account number!\n");
        sleep(2);
        goto invalid;
    }
    if (account_exists(accNbr, db))
    {
        printf("\tAccount number already exists\n\n");
        sleep(2);
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
    safe_int_input(&input);
    if (input < 1 || input > 5)
    {
        system("clear");
        printf("Invalid account type!\n");
        goto invalid;
    }
    strncpy(info->accountType, accountTypes[input - 1], sizeof(info->accountType) - 1);
    info->accountType[sizeof(info->accountType) - 1] = '\0'; /* ensure null-termination */
}

void insert_balance(Record *info)
{
    char input[20];
    char *endptr;
    double balance;

    while (1)
    {
        errno = 0;

        system("clear");
        printf("\t\t\t===== New record =====\n");
        printf("\tEnter balance: ");

        if (safe_string_input(input, 0) != 1)
        {
            printf("✖ Invalid input! Please enter a valid balance.\n");
            sleep(2);
            continue;
        }
        balance = strtod(input, &endptr);
        if (*endptr != '\0' || errno == ERANGE || balance < 0)
        {
            printf("✖ Invalid input! Please enter a valid balance.\n");
            sleep(2);
            continue;
        }
        info->balance = balance;
        break;
    }
}

// Create new account
void create_new_acc(User usr, sqlite3 *db)
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
    info = fill_info(db, usr);
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
    else {
        sqlite3_finalize(stmt);
        printf("New account created successfully!\n");
        //todo: update account count after account creation
        get_acc_nbrs(&usr, db);
        usr.accCount++;
        return;
    }

    sqlite3_finalize(stmt);
}

//  TODO :  **Update account information** function
void update_acc_info(User usr, sqlite3 *db)
{
    if (usr.accCount == 0)
    {
        system("clear");
        printf("No accounts found for user: %s\n", usr.name);
        return;
    }

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
        if (safe_int_input(&choice) != 1)
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

    printf("\t\t====== Update Account Informations ======\n\n");
    printf("\t\tselected account number: %ld\n", usr.accountIds[choice]);
    printf("\t\tOptions:\n\n\t\t"
           "[1] Update Country\n\t\t"
           "[2] Update Phone-Number\n\n\t\t"
           "[3] Exit\n\n\t\t"
           "Your input: ");
    safe_int_input(&prompt);
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
        main_menu(db, usr);
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
void make_transaction(sqlite3 *db, User usr)
{
    system("clear");
    if (usr.accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr.name);
        return;
    }
    int choice, prompt;
    for (int i = 0; i < usr.accCount && usr.accountIds[i]; i++)
        printf("[%d] Account number: %ld\n", i, usr.accountIds[i]);

    while (1)
    {
        choice = -1;
        printf("Enter account number: ");
        if (safe_int_input(&choice) != 1)
            printf("✖ Invalid input! Please enter a valid number.\n");
        else if (choice < 0 || choice > (usr.accCount - 1))
        {
            printf("✖ Invalid option! Please enter a number between 0 and %d.\n", usr.accCount - 1);
        }
        else
            break;
    }
    system("clear");
    char accType[8];
    get_acc_type(db, usr, choice, accType);
    float balance = get_balance(db, usr, choice);
invalid:
    prompt = -1;

    printf("\t\t====== Make Transaction ======\n\n");
    printf("\t\tselected account number: %ld\n", usr.accountIds[choice]);
    printf("\t\tOptions:\n\n\t\t"
           "[1] Withdraw\n\t\t"
           "[2] Diposit\n\n\t\t"
           "[3] Back to menu\n\n\t\t"
           "Your input: ");
    safe_int_input(&prompt);

    if (strncasecmp(accType, "fixed", 5) == 0)
    {
        printf("\nERROR: Cannot modify balance on FIXED accounts.\n");
        return;
    }
    if (prompt == 1)
    {
        withdraw(db, usr, choice, balance);
    }
    else if (prompt == 2)
    {
        deposit(db, usr, choice, balance);
    }
    else if (prompt == 3)
        main_menu(db, usr);
    else
    {
        system("clear");
        printf("\n   | [+] Non-Valid input |\n\n");
        goto invalid;
    }
    sleep_sec(2);
}

int get_acc_type(sqlite3 *db, User usr, int choice, char buffer[8])
{
    const char *sql = "SELECT accType FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (!buffer || sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int(stmt, 2, usr.accountIds[choice]);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *temp = (const char *)sqlite3_column_text(stmt, 0);
        if (temp && strlen(temp) < 8)
        {
            strcpy(buffer, temp);
            sqlite3_finalize(stmt);
            return true;
        }
    }

    sqlite3_finalize(stmt);
    return false;
}

float get_balance(sqlite3 *db, User usr, int choice)
{
    if (!usr.accountIds[choice])
    {
        printf("No accounts found for user: %s\n", usr.name);
        return -1;
    }

    const char *sql = "SELECT balance FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    float balance = 0.0;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return balance;
    }
    sqlite3_bind_int(stmt, 1, usr.id);
    sqlite3_bind_int(stmt, 2, usr.accountIds[choice]);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        balance = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return balance;
}

int withdraw(sqlite3 *db, User usr, int choice, float balance)
{
    const char *sql = "UPDATE records SET balance = ? WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    float withdrawAmount;
    while (1)
    {
        printf("Enter amount to withdraw: ");
        if (safe_float_input(&withdrawAmount) != 1 || withdrawAmount <= 0 || withdrawAmount > balance)
        {
            printf("✖ Invalid input! Please enter a valid amount.\n");
            continue;
        }
        break;
    }
    // Update balance
    balance -= withdrawAmount;

    // Bind parameters
    sqlite3_bind_double(stmt, 1, balance);
    sqlite3_bind_int(stmt, 2, usr.id);
    sqlite3_bind_int64(stmt, 3, usr.accountIds[choice]);

    // Execute
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    system("clear");
    // Print success message
    printf("\t\t\t===== Withdrawal Successful =====\n");
    printf("Account number: %ld\n", usr.accountIds[choice]);
    printf("Withdrawal amount: %.2f\n", withdrawAmount);
    printf("Old balance: %.2f New balance: %.2f\n", balance + withdrawAmount, balance);
    return 0;
}

int deposit(sqlite3 *db, User usr, int choice, float balance)
{
    const char *sql = "UPDATE records SET balance = ? WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;

    // Prepare statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Get deposit amount
    float depositAmount = 0;
    while (1)
    {
        printf("Enter amount to deposit: ");
        if (safe_float_input(&depositAmount) != 1 || depositAmount <= 0)
        {
            printf("✖ Invalid input! Please enter a valid amount.\n");
            continue;
        }
        break;
    }

    balance += depositAmount;

    sqlite3_bind_double(stmt, 1, balance);
    sqlite3_bind_int(stmt, 2, usr.id);
    sqlite3_bind_int64(stmt, 3, usr.accountIds[choice]);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    system("clear");
    // Print success message
    printf("\t\t\t===== Deposit Successful =====\n");
    printf("Account number: %ld\n", usr.accountIds[choice]);
    printf("Deposit amount: %.2f\n", depositAmount);
    printf("Old balance: %.2f New balance: %.2f\n", balance - depositAmount, balance);
    return 0;
}
// TODO : add your **Transfer owner** function
// void TransferOwnership(User *usr, sqlite3 *db)
// {
// if (usr->accCount == 0)
// {
//     printf("No accounts found for user: %s\n", usr->name);
//     return;
// }
// }

// TODO : add your **Check the details of existing accounts** function
void check_existing_accs(User usr, sqlite3 *db)
{
    if (usr.accCount == 0)
    {
        system("clear");
        printf("No accounts found for user: %s\n", usr.name);
        return;
    }

    const char *sql = "SELECT accNbr, created_at, country, phone, balance, accType "
                      "FROM records WHERE userID = ? AND accNbr = ?";
    sqlite3_stmt *stmt;
    int choice;
    // put this into helper function
    system("clear");
    for (int i = 0; i < usr.accCount ; i++)
        printf("[%d] Account number: %ld\n", i, usr.accountIds[i]);

    while (1)
    {
        choice = -1;
        printf("Enter account number: ");
        if (safe_int_input(&choice) != 1)
        {
            printf("✖ Invalid input! Please enter a valid number.\n");
        }
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
        Record info = {0};

        info.accountNbr = sqlite3_column_int64(stmt, 0);

        // Copy deposit safely
        const unsigned char *deposit_text = sqlite3_column_text(stmt, 1);
        if (deposit_text)
        {
            strncpy(info.deposit, (const char *)deposit_text, sizeof(info.deposit) - 1);
            info.deposit[sizeof(info.deposit) - 1] = '\0';
        }

        // Copy country safely
        const unsigned char *country_text = sqlite3_column_text(stmt, 2);
        if (country_text)
        {
            strncpy(info.country, (const char *)country_text, sizeof(info.country) - 1);
            info.country[sizeof(info.country) - 1] = '\0';
        }

        // Copy phone safely
        const unsigned char *phone_text = sqlite3_column_text(stmt, 3);
        if (phone_text)
        {
            strncpy(info.phone, (const char *)phone_text, sizeof(info.phone) - 1);
            info.phone[sizeof(info.phone) - 1] = '\0';
        }

        // Copy accountType safely
        const unsigned char *type_text = sqlite3_column_text(stmt, 5);
        if (type_text)
        {
            strncpy(info.accountType, (const char *)type_text, 20 - 1);
            info.accountType[sizeof(info.accountType) - 1] = '\0';
        }

        info.balance = sqlite3_column_double(stmt, 4);

        print_accounts(info);
        caculate_interest((const unsigned char *)info.accountType, info.balance, info.accountNbr);
    }

    sqlite3_finalize(stmt);
}

void print_accounts(Record rec)
{
    printf("_____________________\n");
    printf("Account number: %ld\n", rec.accountNbr);
    printf("Created Date: %s\n", rec.deposit);
    printf("Country: %s\n", rec.country);
    printf("Phone: %s\n", rec.phone);
    printf("Balance: %.2f\n", rec.balance);
    printf("Account Type: %s\n", rec.accountType);
    printf("_____________________\n");
}

void caculate_interest(const unsigned char *accountType, double balance, long account_nbr)
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

void get_acc_nbrs(User *usr, sqlite3 *db)
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