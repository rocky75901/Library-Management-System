#include "Library.h"
#include <sstream>
#include <iostream>

Library::~Library()
{
    for (auto user : users)
        delete user;
}

void Library::backupFile(const string &filename)
{
    if (ifstream(filename))
    {
        string backupName = filename + ".bak";
        ifstream src(filename, ios::binary);
        ofstream dst(backupName, ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
}

void Library::restoreFromBackup(const string &filename)
{
    string backupName = filename + ".bak";
    if (ifstream(backupName))
    {
        ifstream src(backupName, ios::binary);
        ofstream dst(filename, ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
}

bool Library::isBookBorrowedByUser(const string &userID, const string &bookID) const
{
    for (const auto &record : borrowRecords)
    {
        if (record.getUserID() == userID && record.getBookID() == bookID)
            return true;
    }
    return false;
}

void Library::initializeDefaultData()
{
    cout << "No existing data found. Please add data through the librarian interface or ensure CSV files exist.\n";
}

void Library::loadBooks(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "No existing books file found. Initializing with default books.\n";
        initializeDefaultData();
        saveBooks(filename);
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        stringstream ss(line);
        string bookID, title, author, publisher, yearStr, ISBN, status;
        if (getline(ss, bookID, ',') && getline(ss, title, ',') && getline(ss, author, ',') &&
            getline(ss, publisher, ',') && getline(ss, yearStr, ',') && getline(ss, ISBN, ',') && getline(ss, status))
        {
            try
            {
                int year = stoi(yearStr);
                books.emplace_back(bookID, title, author, publisher, year, ISBN, status);
            }
            catch (const invalid_argument &e)
            {
                cerr << "Error: Invalid year in book record: " << line << endl;
            }
        }
    }
    file.close();
}

void Library::saveBooks(const string &filename)
{
    backupFile(filename);
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        restoreFromBackup(filename);
        return;
    }
    file << "BookID,Title,Author,Publisher,Year,ISBN,Status\n";
    for (const auto &book : books)
    {
        file << book.toCSV() << "\n";
    }
    file.close();
}

void Library::loadUsers(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "No existing users file found. Initializing with default users.\n";
        initializeDefaultData();
        saveUsers(filename);
        return;
    }
    string line;
    getline(file, line); 
    while (getline(file, line))
    {
        stringstream ss(line);
        string userID, name, role, password;
        if (getline(ss, userID, ',') && getline(ss, name, ',') && getline(ss, role, ',') && getline(ss, password))
        {
            if (role == "Student")
                users.push_back(new Student(userID, name, password));
            else if (role == "Faculty")
                users.push_back(new Faculty(userID, name, password));
            else if (role == "Librarian")
                users.push_back(new Librarian(userID, name, password));
        }
    }
    file.close();
}

void Library::saveUsers(const string &filename)
{
    backupFile(filename);
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        restoreFromBackup(filename);
        return;
    }
    file << "UserID,Name,Role,Password\n";
    for (const auto &user : users)
    {
        file << user->toCSV() << "\n";
    }
    file.close();
}

void Library::loadAccounts(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "No existing accounts file found. Initializing with default accounts.\n";
        initializeDefaultData();
        saveAccounts(filename);
        return;
    }
    string line;
    getline(file, line); 
    while (getline(file, line))
    {
        stringstream ss(line);
        string userID, role, currentBorrowedStr, fineAmountStr, borrowHistory;
        if (getline(ss, userID, ',') && getline(ss, role, ',') && getline(ss, currentBorrowedStr, ',') &&
            getline(ss, fineAmountStr, ',') && getline(ss, borrowHistory))
        {
            try
            {
                int currentBorrowed = stoi(currentBorrowedStr);
                double fineAmount = stod(fineAmountStr);
                Account account(userID, role, currentBorrowed, fineAmount);
                stringstream hs(borrowHistory);
                string bookID;
                while (hs >> bookID)
                    account.addBorrowedBook(bookID);
                accounts.push_back(account);
            }
            catch (const invalid_argument &e)
            {
                cerr << "Error: Invalid numeric value in account record: " << line << endl;
            }
        }
    }
    file.close();
}

void Library::saveAccounts(const string &filename)
{
    backupFile(filename);
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        restoreFromBackup(filename);
        return;
    }
    file << "UserID,Role,CurrentBorrowed,FineAmount,BorrowHistory\n";
    for (const auto &account : accounts)
    {
        file << account.toCSV() << "\n";
    }
    file.close();
}

void Library::loadBorrowRecords(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
        return;
    string line;
    getline(file, line); 
    while (getline(file, line))
    {
        stringstream ss(line);
        string userID, bookID, ISBN, borrowMinutesStr, borrowSecondsStr, dueMinutesStr, dueSecondsStr;
        if (getline(ss, userID, ',') && getline(ss, bookID, ',') && getline(ss, ISBN, ',') &&
            getline(ss, borrowMinutesStr, ',') && getline(ss, borrowSecondsStr, ',') &&
            getline(ss, dueMinutesStr, ',') && getline(ss, dueSecondsStr))
        {
            try
            {
                borrowRecords.emplace_back(userID, bookID, ISBN, stoi(borrowMinutesStr), stoi(borrowSecondsStr),
                                           stoi(dueMinutesStr), stoi(dueSecondsStr));
            }
            catch (const invalid_argument &e)
            {
                cerr << "Error: Invalid time in borrow record: " << line << endl;
            }
        }
    }
    file.close();
}

void Library::saveBorrowRecords(const string &filename)
{
    backupFile(filename);
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        restoreFromBackup(filename);
        return;
    }
    file << "UserID,BookID,ISBN,BorrowMinutes,BorrowSeconds,DueMinutes,DueSeconds\n";
    for (const auto &record : borrowRecords)
    {
        file << record.toCSV() << "\n";
    }
    file.close();
}

void Library::loadReservations(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
        return;
    string line;
    getline(file, line); 
    while (getline(file, line))
    {
        stringstream ss(line);
        string userID, bookID, reservationMinutesStr, reservationSecondsStr, activeStr;
        if (getline(ss, userID, ',') && getline(ss, bookID, ',') && getline(ss, reservationMinutesStr, ',') &&
            getline(ss, reservationSecondsStr, ',') && getline(ss, activeStr))
        {
            try
            {
                reservations.emplace_back(userID, bookID, stoi(reservationMinutesStr),
                                          stoi(reservationSecondsStr), activeStr == "1");
            }
            catch (const invalid_argument &e)
            {
                cerr << "Error: Invalid time in reservation record: " << line << endl;
            }
        }
    }
    file.close();
}

void Library::saveReservations(const string &filename)
{
    backupFile(filename);
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        restoreFromBackup(filename);
        return;
    }
    file << "UserID,BookID,ReservationMinutes,ReservationSeconds,Active\n";
    for (const auto &reservation : reservations)
    {
        file << reservation.toCSV() << "\n";
    }
    file.close();
}

User *Library::authenticate(const string &userID, const string &password)
{
    for (auto &user : users)
    {
        if (user->getUserID() == userID && user->getPassword() == password)
            return user;
    }
    return nullptr;
}

Account *Library::findAccount(const string &userID)
{
    for (auto &account : accounts)
    {
        if (account.getUserID() == userID)
            return &account;
    }
    return nullptr;
}

Book *Library::findBook(const string &bookID) const
{
    for (const auto &book : books)
    {
        if (book.getBookID() == bookID)
            return const_cast<Book *>(&book);
    }
    return nullptr;
}

void Library::displayBooks() const
{
    if (books.empty())
    {
        cout << "No books in the library.\n";
        return;
    }
    cout << "\nBook List:\n";
    for (const auto &book : books)
    {
        cout << "ID: " << book.getBookID() << " | Title: " << book.getTitle() << " | Author: " << book.getAuthor() << " | Status: '" << book.getStatus() << "'\n";
    }
}

vector<Book> Library::searchBooks(const string &query) const
{
    vector<Book> results;
    for (const auto &book : books)
    {
        if (book.getTitle().find(query) != string::npos || book.getAuthor().find(query) != string::npos ||
            book.getISBN().find(query) != string::npos)
        {
            results.push_back(book);
        }
    }
    return results;
}

bool Library::reserveBook(const string &userID, const string &bookID)
{
    Book *book = findBook(bookID);
    Account *account = findAccount(userID);

    if (!book || !account)
    {
        cout << "Book or user not found.\n";
        return false;
    }

    string status = book->getStatus();
    status.erase(0, status.find_first_not_of(" \t\n\r\f\v"));
    status.erase(status.find_last_not_of(" \t\n\r\f\v") + 1);

    if (status != "Borrowed")
    {
        cout << "Cannot reserve: Book must be currently borrowed. Current status is '" << status << "'.\n";
        return false;
    }

    if (account->getFineAmount() > 0)
    {
        cout << "Cannot reserve: Unpaid fines pending.\n";
        return false;
    }

    int activeReservations = 0;
    for (const auto &reservation : reservations)
    {
        if (reservation.getUserID() == userID && reservation.isActive())
        {
            activeReservations++;
        }
    }

    int maxReservations = (account->getRole() == "Student") ? 3 : (account->getRole() == "Faculty") ? 5
                                                                                                    : 0;
    if (activeReservations >= maxReservations)
    {
        cout << "Cannot reserve: Maximum reservation limit (" << maxReservations << ") reached. You currently have "
             << activeReservations << " active reservations.\n";
        return false;
    }

    for (const auto &reservation : reservations)
    {
        if (reservation.getBookID() == bookID && reservation.isActive())
        {
            cout << "Cannot reserve: This book is already reserved by another user.\n";
            return false;
        }
    }

    if (isBookBorrowedByUser(userID, bookID))
    {
        cout << "Cannot reserve: You have already borrowed this book.\n";
        return false;
    }

    int minutes, seconds;
    getCurrentTime(minutes, seconds);
    reservations.emplace_back(userID, bookID, minutes, seconds, true);
    book->setStatus("Reserved");
    cout << "Book reserved successfully. You now have " << activeReservations + 1 << " of " << maxReservations << " allowed reservations.\n";
    return true;
}

void Library::cancelReservation(const string &userID, const string &bookID)
{
    Book *book = findBook(bookID);
    for (auto &reservation : reservations)
    {
        if (reservation.getUserID() == userID && reservation.getBookID() == bookID && reservation.isActive())
        {
            reservation.setActive(false);
            if (book && book->getStatus() == "Reserved")
                book->setStatus("Borrowed"); 
            cout << "Reservation cancelled successfully.\n";
            return;
        }
    }
    cout << "No active reservation found for this book.\n";
}

void Library::displayReservations(const string &userID) const
{
    bool found = false;
    for (const auto &reservation : reservations)
    {
        if (reservation.getUserID() == userID && reservation.isActive())
        {
            if (!found)
            {
                cout << "\nYour Active Reservations:\n";
                found = true;
            }
            Book *book = findBook(reservation.getBookID());
            if (book)
                cout << "Book: " << book->getTitle() << " (ID: " << book->getBookID() << ")\n";
        }
    }
    if (!found)
        cout << "You have no active reservations.\n";
}