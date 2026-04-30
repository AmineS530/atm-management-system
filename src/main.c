#include "atm_sys.h"

int main()
{
    init_ui();
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

    end_ui();
    return 0;
}
