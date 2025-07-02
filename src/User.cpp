#include "User.h"
#include "TimeUtils.h"
#include <algorithm>

User::User(string id, string n, string r, string p) : userID(id), name(n), role(r), password(p) {}

Student::Student(string id, string n, string p) : User(id, n, "Student", p) {}

void Student::borrowBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations)
{
    string status = book.getStatus();
    status.erase(0, status.find_first_not_of(" \t\n\r\f\v"));
    status.erase(status.find_last_not_of(" \t\n\r\f\v") + 1);

    if (status == "Available" && account.getCurrentBorrowed() < maxBooks && account.getFineAmount() == 0)
    {
        book.setStatus("Borrowed");
        account.addBorrowedBook(book.getBookID());

        int minutes, seconds;
        getCurrentTime(minutes, seconds);
        int dueMinutes = minutes, dueSeconds = seconds;
        addSimulatedDays(dueMinutes, dueSeconds, maxBorrowingPeriod);
        records.emplace_back(userID, book.getBookID(), book.getISBN(), minutes, seconds, dueMinutes, dueSeconds);

        for (auto &res : reservations)
        {
            if (res.getUserID() == userID && res.getBookID() == book.getBookID() && res.isActive())
            {
                res.setActive(false);
                break;
            }
        }
        cout << "Book borrowed successfully by student. Due in " << maxBorrowingPeriod << " simulated days (1 minute).\n";
    }
    else
    {
        cout << "Cannot borrow: ";
        if (status != "Available")
            cout << "Book status is '" << status << "'";
        else if (account.getCurrentBorrowed() >= maxBooks)
            cout << "Max book limit reached (Current: " << account.getCurrentBorrowed() << ", Max: " << maxBooks << ")";
        else if (account.getFineAmount() > 0)
            cout << "Unpaid fines pending (Amount: " << account.getFineAmount() << ")";
        cout << endl;
    }
}

void Student::returnBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations)
{
    bool isBorrowed = false;
    for (const auto &record : records)
    {
        if (record.getUserID() == userID && record.getBookID() == book.getBookID())
        {
            isBorrowed = true;
            break;
        }
    }

    if (isBorrowed)
    {
        bool hasActiveReservation = false;
        for (auto &reservation : reservations)
        {
            if (reservation.getBookID() == book.getBookID() && reservation.isActive())
            {
                hasActiveReservation = true;
                break;
            }
        }

        if (hasActiveReservation)
        {
            book.setStatus("Reserved");
        }
        else
        {
            book.setStatus("Available");
        }

        account.removeBorrowedBook(book.getBookID());
        for (auto it = records.begin(); it != records.end(); ++it)
        {
            if (it->getUserID() == userID && it->getBookID() == book.getBookID())
            {
                if (it->isOverdue())
                {
                    int overdueDays = it->calculateOverdueDays();
                    account.calculateFines(overdueDays, fineRate);
                    cout << "Book returned. Overdue by " << overdueDays << " simulated days. Fine: " << overdueDays * fineRate << " rupees.\n";
                }
                else
                {
                    cout << "Book returned successfully.\n";
                }
                records.erase(it);
                break;
            }
        }
    }
    else
    {
        cout << "Book not borrowed by you.\n";
    }
}

Faculty::Faculty(string id, string n, string p) : User(id, n, "Faculty", p) {}

void Faculty::borrowBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations)
{
    string status = book.getStatus();
    status.erase(0, status.find_first_not_of(" \t\n\r\f\v"));
    status.erase(status.find_last_not_of(" \t\n\r\f\v") + 1);

    bool hasOverdue = false;
    for (const auto &record : records)
    {
        if (record.getUserID() == userID && record.isOverdue() && record.calculateOverdueDays() > 4)
        {
            hasOverdue = true;
            break;
        }
    }

    if (status == "Available" && account.getCurrentBorrowed() < maxBooks && !hasOverdue)
    {

        book.setStatus("Borrowed");
        account.addBorrowedBook(book.getBookID());

        int minutes, seconds;
        getCurrentTime(minutes, seconds);
        int dueMinutes = minutes, dueSeconds = seconds;
        addSimulatedDays(dueMinutes, dueSeconds, maxBorrowingPeriod);
        records.emplace_back(userID, book.getBookID(), book.getISBN(), minutes, seconds, dueMinutes, dueSeconds);

        for (auto &res : reservations)
        {
            if (res.getUserID() == userID && res.getBookID() == book.getBookID() && res.isActive())
            {
                res.setActive(false);
                break;
            }
        }
        cout << "Book borrowed successfully by faculty. Due in " << maxBorrowingPeriod << " simulated days (2 minutes).\n";
    }
    else
    {
        cout << "Cannot borrow: ";
        if (status != "Available")
            cout << "Book status is '" << status << "'";
        else if (account.getCurrentBorrowed() >= maxBooks)
            cout << "Max book limit reached (Current: " << account.getCurrentBorrowed() << ", Max: " << maxBooks << ")";
        else if (hasOverdue)
            cout << "Overdue books > 4 minutes restrict borrowing";
        cout << endl;
    }
}

void Faculty::returnBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations)
{
    bool isBorrowed = false;
    for (const auto &record : records)
    {
        if (record.getUserID() == userID && record.getBookID() == book.getBookID())
        {
            isBorrowed = true;
            break;
        }
    }

    if (isBorrowed)
    {
        bool hasActiveReservation = false;
        for (auto &reservation : reservations)
        {
            if (reservation.getBookID() == book.getBookID() && reservation.isActive())
            {
                hasActiveReservation = true;
                break;
            }
        }

        if (!hasActiveReservation)
        {
            book.setStatus("Available");
        }

        account.removeBorrowedBook(book.getBookID());
        for (auto it = records.begin(); it != records.end(); ++it)
        {
            if (it->getUserID() == userID && it->getBookID() == book.getBookID())
            {
                if (it->isOverdue() && it->calculateOverdueDays() > 4)
                {
                    cout << "Book returned. Note: It was overdue by more than 4 minutes.\n";
                }
                else
                {
                    cout << "Book returned successfully.\n";
                }
                records.erase(it);
                break;
            }
        }
    }
    else
    {
        cout << "Book not borrowed by you.\n";
    }
}

Librarian::Librarian(string id, string n, string p) : User(id, n, "Librarian", p) {}

bool Librarian::isValidISBN(const string &isbn) const
{
    string cleanISBN = isbn;
    cleanISBN.erase(remove(cleanISBN.begin(), cleanISBN.end(), '-'), cleanISBN.end());
    cleanISBN.erase(remove(cleanISBN.begin(), cleanISBN.end(), ' '), cleanISBN.end());
    if (cleanISBN.length() != 10 && cleanISBN.length() != 13)
        return false;
    for (char c : cleanISBN)
        if (!isdigit(c))
            return false;
    return true;
}

void Librarian::addBook(vector<Book> &books, const Book &book)
{
    if (!isValidISBN(book.getISBN()))
    {
        cout << "Error: Invalid ISBN format. ISBN must be 10 or 13 digits.\n";
        return;
    }
    for (const auto &b : books)
    {
        if (b.getISBN() == book.getISBN())
        {
            cout << "Error: Book with this ISBN already exists.\n";
            return;
        }
    }
    books.push_back(book);
    cout << "Book added successfully.\n";
}

void Librarian::removeBook(vector<Book> &books, const string &bookID)
{
    auto it = remove_if(books.begin(), books.end(), [&](const Book &b)
                        { return b.getBookID() == bookID; });
    if (it != books.end())
    {
        books.erase(it, books.end());
        cout << "Book removed by librarian.\n";
    }
    else
    {
        cout << "Book not found.\n";
    }
}

void Librarian::addUser(vector<User *> &users, vector<Account> &accounts, const string &userID, const string &name, const string &role, const string &password)
{
    if (userID.empty() || name.empty() || role.empty() || password.empty())
    {
        cout << "Error: All fields must be filled.\n";
        return;
    }
    if (password.length() < 6)
    {
        cout << "Error: Password must be at least 6 characters long.\n";
        return;
    }
    for (const auto &user : users)
    {
        if (user->getUserID() == userID)
        {
            cout << "Error: UserID already exists.\n";
            return;
        }
    }
    if (role == "Student")
    {
        users.push_back(new Student(userID, name, password));
        accounts.emplace_back(userID, "Student");
    }
    else if (role == "Faculty")
    {
        users.push_back(new Faculty(userID, name, password));
        accounts.emplace_back(userID, "Faculty");
    }
    else if (role == "Librarian")
    {
        users.push_back(new Librarian(userID, name, password));
        accounts.emplace_back(userID, "Librarian");
    }
    else
    {
        cout << "Invalid role. Use Student, Faculty, or Librarian.\n";
        return;
    }
    cout << "User " << userID << " added successfully.\n";
}

void Librarian::removeUser(vector<User *> &users, vector<Account> &accounts, const string &userID)
{
    auto it = find_if(users.begin(), users.end(), [&](const User *u)
                      { return u->getUserID() == userID; });
    if (it != users.end())
    {
        delete *it;
        users.erase(it);
        auto accIt = remove_if(accounts.begin(), accounts.end(), [&](const Account &a)
                               { return a.getUserID() == userID; });
        if (accIt != accounts.end())
        {
            accounts.erase(accIt, accounts.end());
            cout << "User " << userID << " removed successfully.\n";
        }
    }
    else
    {
        cout << "User not found.\n";
    }
}

void Librarian::borrowBook(Book &, Account &, vector<BorrowRecord> &, vector<Reservation> &)
{
    cout << "Librarians cannot borrow books.\n";
}

void Librarian::returnBook(Book &, Account &, vector<BorrowRecord> &, vector<Reservation> &)
{
    cout << "Librarians cannot return books.\n";
}

void User::cancelReservation(vector<Reservation> &reservations, const string &bookID)
{
    for (auto &reservation : reservations)
    {
        if (reservation.getUserID() == userID && reservation.getBookID() == bookID && reservation.isActive())
        {
            reservation.setActive(false);
            cout << "Reservation for book " << bookID << " canceled successfully.\n";
            return;
        }
    }
    cout << "No active reservation found for book " << bookID << ".\n";
}