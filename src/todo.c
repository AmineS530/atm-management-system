#include "atm_sys.h"
// account types
// place holder : https://stackoverflow.com/questions/5309859/how-to-define-an-array-of-functions-in-c

//     printf("\nEnter today's date(mm/dd/yyyy):");
//     scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

// TODO : add your **Transfer owner** function
void TransferOwnership(User *usr, sqlite3 *db)
{
    if (usr->accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr->name);
        return;
    }
}

// TODO : add your **Remove existing account** function
void RemoveAcc(User *usr, sqlite3 *db)
{
    if (usr->accCount == 0)
    {
        printf("No accounts found for user: %s\n", usr->name);
        return;
    }
}
