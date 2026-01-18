#include "helpers.h"

void stay_or_return(sqlite3 *db, int notGood, void f(sqlite3 *db, User u), User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        safe_int_input(&option);
        if (option == 0)
            f(db, u);
        else if (option == 1)
            main_menu(db, u);
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
        safe_int_input(&option);
    }
    if (option == 1)
        main_menu(db, u);
    else
    {
        system("clear");
        exit(1);
    }
}

void success()
{
    //todo: rework
    int option = -1;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    safe_int_input(&option);
    system("clear");
    printf("\noption: %d\n", option);
    sleep_sec(2);
    if (option == 1)
        return;
    else if (option == 0)
        exit(0);
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}
