#include "../atm_sys.h"

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
    int choice = select_account(usr);

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