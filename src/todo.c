#include "atm_sys.h"

void regMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\tUser Login: ");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

// initializing
void SetData(Data *sqlData)
{
    int resCode = sqlite3_open("data/DATA.db", &sqlData->db);
    if (resCode != SQLITE_OK)
    {
        fprintf(stderr, "error: %s", sqlite3_errmsg(sqlData->db));
        sqlite3_close(sqlData->db);
        return 1;
    }
}
// psudo funcs
//  TODO : add your **Update account information** function
void UpdateAccInfo(User u)
{
}

// TODO : add your **Make transaction** function
void MakeTransaction(User u)
{
}
// TODO : add your **Transfer owner** function
void TransferOwnership(User u)
{
}
// TODO : add your **Check the details of existing accounts** function
void CheckExistingaccs(User u)
{
}
// TODO : add your **Remove existing account** function
void RemoveAcc(User u)
{
}