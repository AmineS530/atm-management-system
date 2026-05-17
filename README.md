# ATM Management System (Ncurses Edition)

## 📌 Overview
The ATM Management System is a robust, C-based terminal application that simulates a real-world ATM interface. Upgraded from a standard command-line tool, it now features a rich Text-Based User Interface (TUI) powered by **Ncurses**, providing a "GUI-like" experience right in the terminal with windows, colors, and arrow-key navigation. The backend is powered by **SQLite** for secure data storage and retrieval.

## 🚀 Features
- **Secure Authentication:** Custom password hashing and salted storage for user accounts.
- **Interactive TUI:** Ncurses-based interface with menus, popups, color themes, and arrow-key navigation.
- **Database Driven:** Robust data management using SQLite3 for users, accounts, and transaction records.
- **Account Management:**
  - Create new accounts (Current, Savings, Fixed).
  - Update account details.
  - Remove existing accounts.
  - Transfer ownership of accounts.
- **Financial Operations:**
  - Make transactions (Deposits/Withdrawals).
  - Check existing accounts and view balances.
  - View all accounts associated with a user.
  - Calculate maturity interest for Fixed accounts and monthly interest for Savings.
- **Safe Input Handling:** Buffer-overflow protected input fields for secure and stable data entry.

## 📂 Project Structure
```text
├── data
│   ├── DATA.db
│   ├── records.txt
│   ├── schema.sql
│   └── users.txt
├── Makefile
├── src
│   ├── atm_sys.h
│   ├── auth
│   │   ├── auth.c
│   │   ├── auth.h
│   │   └── encrypt.c
│   ├── helpers
│   │   ├── account_logging.c
│   │   ├── errors.c
│   │   ├── helpers.h
│   │   ├── init.c
│   │   ├── logs.c
│   │   ├── query_helpers.c
│   │   ├── system.c
│   │   └── utils.c
│   ├── input
│   │   ├── input.h
│   │   ├── input_utils.c
│   │   └── safe_input_utils.c
│   ├── main.c
│   ├── menus
│   │   ├── check_all_accs.c
│   │   ├── check_existing_accs.c
│   │   ├── create_account.c
│   │   ├── make_transaction.c
│   │   ├── menus.c
│   │   ├── menus.h
│   │   ├── remove_acc.c
│   │   ├── transfer_ownership.c
│   │   └── update_acc.c
│   ├── sqlite
│   │   ├── libsqlite3.a
│   │   ├── Makefile
│   │   ├── sqlite3ext.h
│   │   └── sqlite3.h
│   └── ui
│       ├── ui.c
│       └── ui.h
└── z1_ATM.exec
```

## 🛠️ Build & Run Instructions

### Prerequisites
- GCC Compiler
- Ncurses Library (e.g., `libncurses-dev` or `ncurses-devel`)
- Make

### Compilation
To compile the project and build the SQLite library dependency:
```bash
make all
```

### Execution
Run the compiled executable:
```bash
./z1_ATM.exec
```

### Cleanup
To remove compiled object files:
```bash
make clean
```
To remove the executable and reset the environment:
```bash
make fclean
```
