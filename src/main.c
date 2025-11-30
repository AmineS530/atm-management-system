#include "atm_sys.h"

int main()
{
    sqlite3 *db = init();
    User usr = init_user();
    while (1) {
        int rc = init_menu(db, &usr);

        if (rc == INIT_MENU_GO)
        {
            main_menu(db, usr);
            continue;
        }
        break;
    }

    return 0;
}
