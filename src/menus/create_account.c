#include "menus.h"

const char *accountTypes[5] = {"Current", "Savings", "Fixed01", "Fixed02", "Fixed03"};

static Record fill_info(sqlite3 *db, User usr);
static void get_full_name(Record *info);
static void get_account_nbr(Record *info, sqlite3 *db);
static void get_account_type(Record *info);
static void insert_balance(Record *info);
static int read_number(const char *label, int *out, int min, int max);
static void get_date(char *deposit_date);

// Create new account
void create_new_acc(User *usr, sqlite3 *db)
{
    if (usr->accCount >= MAX_ACCOUNTS)
    {
        printf("You have reached the maximum number of accounts you can create.\n");
        sleep_sec(3);
        return;
    }

    // SQL query to insert data into the `records` table
    const char *sql = "INSERT INTO records (userID, accNbr, fullname, country, phone, balance, accType, deposit_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    Record info;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        // printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        log_error(usr->name, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        return;
    }

    // Call a helper function to fill the Record structure
    info = fill_info(db, *usr);
    // Bind values to the prepared statement
    sqlite3_bind_int(stmt, 1, usr->id);
    sqlite3_bind_int64(stmt, 2, info.accountNbr);
    sqlite3_bind_text(stmt, 3, info.name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, info.country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, info.phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 6, info.balance);
    sqlite3_bind_text(stmt, 7, info.accountType, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, info.deposit, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        log_error(usr->name, "Error executing statement: %s", sqlite3_errmsg(db));
    }
    else
    {
        sqlite3_finalize(stmt);
        printf("New account created successfully!\n");
        log_info(usr->name, "New account created number: %ld", info.accountNbr);
        get_acc_nbrs(usr, db);
        return;
    }
}

static Record fill_info(sqlite3 *db, User usr)
{
    Record info = {0};

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
        sleep_sec(2);
    }
    while (1)
    {
        system("clear");
        printf("\t\t\t===== New record =====\n");
        if (check_country(info.country))
            break;
        system("clear");
        printf("Country is not valid!\n");
        sleep_sec(2);
    }
    get_account_type(&info);
    insert_balance(&info);
    return info;
}

static void get_full_name(Record *info)
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

static void get_account_nbr(Record *info, sqlite3 *db)
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

static void get_account_type(Record *info)
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

static void insert_balance(Record *info)
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
// Generic validator: ask for a number within [min, max]
static int read_number(const char *label, int *out, int min, int max)
{
    while (1)
    {
        printf("%s: ", label);

        if (!safe_int_input(out))
        {
            system("clear");
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        if (*out < min || *out > max)
        {
            system("clear");
            printf("Invalid %s! Enter a value between %d and %d.\n", label, min, max);
            continue;
        }

        return 1;
    }
}

static void get_date(char *deposit_date)
{
    struct tm set_time = {0};

    system("clear");
    printf("Enter the date!\n");

    read_number("Day", &set_time.tm_mday, 1, 31);
    read_number("Month", &set_time.tm_mon, 1, 12);
    read_number("Year", &set_time.tm_year, 1980, 2025);

    set_time.tm_mon -= 1;     // 0–11
    set_time.tm_year -= 1900; // years since 1900

    strftime(deposit_date, 30, "%d/%m/%Y", &set_time);

    printf("Entered Date: %s\n", deposit_date);
    sleep(2);
}
