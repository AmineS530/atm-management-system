# 🚀 ATM System Upgrade: Ncurses Edition (AuDHD Guide)

### 🧠 The Big Picture (Executive Summary)
We took a basic terminal app and gave it a **GUI-like soul** inside the terminal. Instead of typing numbers and getting a "wall of text," we now have windows, colors, and arrow-key navigation. This makes it look like a professional tool from the 90s (think BIOS or old server tools).

---

### 🛠 What changed and WHY?

#### 1. The "Face" (UI Abstraction Layer)
*   **What:** Created `src/ui/ui.c` and `ui.h`.
*   **Why:** Ncurses is a "low-level" beast. If we put UI code directly in the logic, the code becomes a mess. 
*   **AuDHD Logic:** I built "Lego bricks" (`show_menu`, `show_error`). Now the rest of the code just says "Show a menu with these options," and the UI layer handles the math of where to draw the box.

#### 2. The "Brains" (Logic vs. UI Separation)
*   **What:** Stripped `printf` and `scanf` from everywhere.
*   **Why:** `printf` is "dumb"—it just spits text. Ncurses is "smart"—it knows exactly where every character is on the screen.
*   **The Fix:** Every menu now uses `show_menu()`. You use **Arrow Keys** to move. This stops users from typing "abc" when the app expects "1", which usually crashes basic C apps.

#### 3. The "Math Fix" (Interest Rates)
*   **What:** Rewrote the interest calculation in `check_existing_accs.c`.
*   **Why:** The original code was lazy—it showed monthly interest for everything. The audit requirements specifically asked for **Maturity Interest** on Fixed accounts.
*   **The Change:** 
    *   **Savings:** Still monthly (7%).
    *   **Fixed:** Now calculates the **Total** after 1, 2, or 3 years and shows the **Exact Future Date**.

#### 4. The "Safety" (Input Handling)
*   **What:** Replaced manual input with `get_input_field`.
*   **Why:** No more "dangling newlines" or buffer overflows. Ncurses handles the string capture cleanly.

---

### 🔍 Deep Dive: File-by-File Changes

#### 📂 `Makefile` (The Builder)
*   **Old:** Compiled standard C files.
*   **New:** Added `LDFLAGS = -lncurses`. 
*   **Logic:** Tells the compiler to "borrow" the Ncurses library during building. Added `src/ui/ui.c` to the source list so it's included in the final executable.

#### 📂 `main.c` (The Lifecycle)
*   **Addition:** `init_ui()` at the start; `end_ui()` at the end.
*   **Logic:** 
    *   `initscr()`: Starts the engine.
    *   `endwin()`: **CRITICAL.** If you forget this, the terminal will be broken (garbled text) after the program closes. This ensures a clean exit.

#### 📂 `src/ui/ui.c` (The Engine) - *Brand New*
*   **`show_menu()`**: Uses a `while(1)` loop to capture `KEY_UP` and `KEY_DOWN`. It toggles `A_REVERSE` (highlight color) on the selected line.
*   **`draw_box()`**: Uses `box()` to draw borders and `mvwprintw()` to center the title.
*   **`get_input_field()`**: Toggles `echo()` (see what you type) and `noecho()` (for passwords). It uses `mvwgetnstr()` which is safe—it stops taking input after X characters to prevent crashes.

#### 📂 `src/menus/check_existing_accs.c` (Interest Logic)
*   **Old:** Linear `printf` that calculated everything as `balance * rate / 12`.
*   **New:** Surgical refactor.
    *   Uses `sscanf(deposit, "%d/%d/%d", &day, &month, &year)` to "explode" the date string into integers.
    *   If it's a **Fixed** account, it adds the years (1, 2, or 3) to the `year` variable and displays the *future* date.
    *   Calculates `total_interest = balance * rate * years` instead of monthly chunks.

#### 📂 `src/helpers/utils.c` (Selection Logic)
*   **Old:** Asked user to type a number to pick an account.
*   **New:** `select_account` now dynamically generates an array of strings (the account numbers) and passes them to the TUI `show_menu`.
*   **Logic:** It converts numbers to strings using `snprintf` so the Ncurses menu can display them.

#### 📂 `src/menus/menus.c` (Flow Control)
*   **Change:** Removed all `termios.h` code. 
*   **Why:** Termios was a hack used to hide passwords in a standard terminal. Ncurses handles this natively with `noecho()`, making the code much shorter and cleaner.

---

### 🗣 How to explain this during Corrections (The "Cheatsheet")

**If they ask: "Why did you use Ncurses?"**
> "I wanted a 'Relational TUI' that felt like a real application. Ncurses allowed me to use windows and colors, making the interface more intuitive than a simple command-line list. It also allowed me to implement arrow-key navigation, which improves the UX."

**If they ask: "How does your UI work?"**
> "I built an abstraction layer in `ui.c`. It encapsulates all the Ncurses initialization and window math. This keeps the business logic (like transactions) separate from the display logic. It makes the code more modular and easier to maintain."

**If they ask: "What did you improve in the logic?"**
> "I fixed the interest calculation to match the specific audit requirements. I implemented the logic to differentiate between monthly savings interest and maturity-based fixed interest, including calculating the specific maturity date based on the deposit date."

---

### 🏗 Architecture Map (For your brain)
*   **`main.c`**: The Boss. Starts Ncurses, then runs the loop.
*   **`src/ui/`**: The Artist. Knows how to draw boxes and handle colors.
*   **`src/menus/`**: The Managers. They ask the Artist to draw menus and then tell the Database what to do.
*   **`src/auth/`**: The Guard. Handles the salted hashing (Security!).

---

### 🎨 Visual Theme
*   **Cyan Boxes:** General info.
*   **Green Popups:** "Yay, it worked!"
*   **Red Popups:** "Oops, something went wrong."
*   **Reverse Video:** The highlight bar you move with arrows.
