#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Book.h"
#include "Account.h"
#include "BorrowRecord.h"
#include "Reservation.h"

using namespace std;

class User
{
protected:
    string userID, name, role, password;

public:
    User(string id = "", string n = "", string r = "", string p = "");
    virtual ~User() = default; // Virtual destructor for proper cleanup
    virtual void borrowBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) = 0;
    virtual void returnBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) = 0;
    virtual int getMaxBooks() const = 0;
    virtual int getMaxBorrowingPeriod() const = 0;
    virtual int getFineRate() const = 0;
    string getUserID() const { return userID; }
    string getName() const { return name; }
    string getRole() const { return role; }
    string getPassword() const { return password; }
    void setName(string n) { name = n; }
    void setPassword(string p) { password = p; }
    string toCSV() const { return userID + "," + name + "," + role + "," + password; }
    void cancelReservation(vector<Reservation> &reservations, const string &bookID);
};

class Student : public User
{
private:
    int maxBooks = 3;
    int maxBorrowingPeriod = 15;
    int fineRate = 10;

public:
    Student(string id = "", string n = "", string p = "");
    void borrowBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) override;
    void returnBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) override;
    int getMaxBooks() const override { return maxBooks; }
    int getMaxBorrowingPeriod() const override { return maxBorrowingPeriod; }
    int getFineRate() const override { return fineRate; }
};

class Faculty : public User
{
private:
    int maxBooks = 5;
    int maxBorrowingPeriod = 30;
    int fineRate = 0;

public:
    Faculty(string id = "", string n = "", string p = "");
    void borrowBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) override;
    void returnBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) override;
    int getMaxBooks() const override { return maxBooks; }
    int getMaxBorrowingPeriod() const override { return maxBorrowingPeriod; }
    int getFineRate() const override { return fineRate; }
};

class Librarian : public User
{
private:
    bool isValidISBN(const string &isbn) const;

public:
    Librarian(string id = "", string n = "", string p = "");
    void addBook(vector<Book> &books, const Book &book);
    void removeBook(vector<Book> &books, const string &bookID);
    void addUser(vector<User *> &users, vector<Account> &accounts, const string &userID, const string &name, const string &role, const string &password);
    void removeUser(vector<User *> &users, vector<Account> &accounts, const string &userID);
    void borrowBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) override;
    void returnBook(Book &book, Account &account, vector<BorrowRecord> &records, vector<Reservation> &reservations) override;
    int getMaxBooks() const override { return 0; }
    int getMaxBorrowingPeriod() const override { return 0; }
    int getFineRate() const override { return 0; }
};

#endif // USER_H