# ATM Management System (Ncurses Edition)

A feature-rich ATM simulation built in **C**, featuring a modern terminal-based interface using **Ncurses** and persistent storage powered by **SQLite3**. This project extends the original ATM assignment requirements with a cleaner architecture, secure authentication, improved user experience, and real-time account ownership notifications.

---

# ✨ Features

## 🔐 Authentication & Security
- User registration and login system
- Unique username enforcement
- Password encryption/hashing before storage
- Secure input handling with buffer overflow protection

## 🖥️ Interactive Terminal UI
- Built with **Ncurses**
- Window-based TUI experience
- Arrow-key navigation
- Color themes and popup dialogs
- Improved terminal responsiveness and layout handling

## 💾 Database System
- SQLite3 relational database integration
- Persistent storage for:
  - Users
  - Accounts
  - Transactions
- Structured schema management

## 🏦 Account Management
- Create accounts:
  - Current
  - Savings
  - Fixed01
  - Fixed02
  - Fixed03
- Update account information:
  - Country
  - Phone number
- Remove accounts
- Transfer account ownership between users

## 💸 Financial Operations
- Deposit money
- Withdraw money
- Balance validation
- Transaction history logging
- Restrictions for fixed accounts
- Savings and fixed account interest calculations

## ⚡ Bonus Features
- Instant ownership transfer notifications between online users
- Optimized and modular codebase
- Custom Makefile
- Advanced TUI compared to the original project specification

---

# 📸 Overview

The project was developed as an enhanced implementation of the original ATM assignment requirements. The original project specification required a basic file-based ATM system in C with features such as registration, account management, transactions, and ownership transfers.

This implementation expands upon those requirements by introducing:
- A complete Ncurses terminal UI
- SQLite relational database support
- Password encryption
- Improved project architecture
- Real-time terminal notifications
- Safer input and error handling

---

# 🧱 Project Structure

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