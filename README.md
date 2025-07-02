# 📚 Library Management System

A robust console-based Library Management System built using C++. It helps manage books, users, borrowing records, reservations, and fines with multi-role support (Students, Faculty, Librarians). All data is stored persistently using CSV files.

## 📖 Table of Contents

- [Features](#features)
- [Setup](#setup)
- [Run Instructions](#run-instructions)
- [Data Format](#data-format)
- [Usage](#usage)
- [File Structure](#file-structure)
- [Important Instructions](#important-instructions)
- [Contact](#contact)

## Features

- Multi-role access: Students, Faculty, Librarians
- Book borrowing, returning, and reservation system
- Fine calculation and payment functionality
- Track overdue books and borrowing history
- Librarian-specific privileges: add/remove books & users
- File-based data persistence with automatic backups
- Console-based interface with intuitive menus

## Setup

1. **Clone the Repository**
   ```sh
   git clone <your-repo-url>
   cd <repo-folder>
   ```
2. **Ensure a C++ compiler is installed** (e.g., g++)

## Run Instructions

### Option 1: Linux/Mac (Recommended)

```sh
g++ src/*.cpp -Iinclude -o library
./library
```

### Option 2: Windows

```sh
g++ src/*.cpp -Iinclude -o library.exe
./library.exe

📂 Data Format
books.csv
BookID,Title,Author,Publisher,Year,ISBN,Status  
B101,The Pragmatic Programmer,Andrew Hunt & David Thomas,Addison-Wesley,1999,9780201616224,Available
```

- Status can be `Available`, `Borrowed`, or `Reserved`

### users.csv

```csv
UserID,Name,Role,Password
S101,Alice,Student,pass101
```

- Role can be `Student`, `Faculty`, or `Librarian`

### accounts.csv

```csv
UserID,Role,CurrentBorrowed,FineAmount,BorrowHistory
S101,Student,2,0.0,B101 B102
```

### borrowed_books.csv

```csv
UserID,BookID,ISBN,BorrowMinutes,BorrowSeconds,DueMinutes,DueSeconds
S101,B110,9780262046305,1016,1,1017,1
```

### reservations.csv

```csv
UserID,BookID,ReservationMinutes,ReservationSeconds,Active
S101,B102,1017,5,1
```

- Active: `1` = active, `0` = inactive

## Usage

### 🔐 Login

Upon running the program, you'll be prompted for:

- UserID
- Password

### 👨‍🎓 Student Menu

- Search Books
- Borrow Book
- Return Book
- View Account
- Pay Fines
- Reserve Book
- Cancel Reservation
- View Reservations
- Logout

### 👩‍🏫 Faculty Menu

(Same as Student except for fines)

- Search Books
- Borrow Book
- Return Book
- View Account
- Reserve Book
- Cancel Reservation
- View Reservations
- Logout

### 🧑‍💼 Librarian Menu

- Add Book
- Remove Book
- View Books
- Add User
- Remove User
- Search Books
- Logout

💡 Use menu numbers to interact with the system. All changes are saved automatically to CSV files.

## File Structure

```
project-root/
├── src/
│   ├── main.cpp
│   ├── Library.cpp
│   ├── User.cpp
│   ├── Book.cpp
│   ├── Account.cpp
│   ├── BorrowRecord.cpp
│   ├── Reservation.cpp
│   └── TimeUtils.cpp
├── include/
│   ├── Library.h
│   ├── User.h
│   ├── Book.h
│   ├── Account.h
│   ├── BorrowRecord.h
│   ├── Reservation.h
│   └── TimeUtils.h
├── books.csv
├── users.csv
├── accounts.csv
├── borrowed_books.csv
├── reservations.csv
├── .vscode/         # (Optional) VS Code settings
├── README.md        # Documentation
└── library.exe      # Compiled Executable (Windows)
```

## Important Instructions

- Do not edit the CSV files manually. Use the interface.
- Data files are backed up automatically with `.bak` extensions.
- User IDs (e.g., S101) and Book IDs (e.g., B101) must be unique.
- Fines and borrow limits are enforced based on roles.
- If data files are missing, you'll be prompted to create them via the librarian interface.
- All values (UserIDs, BookIDs, ISBNs) are treated as strings.
- If you're testing the application, make sure to populate the following files with valid sample data: `books.csv`, `users.csv`, `accounts.csv`, `borrowed_books.csv`, `reservations.csv`

## Contact

If you have any questions, suggestions, or need help:
📧 Email: vrakesh23@iitk.ac.in
