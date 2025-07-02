#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include <fstream>
#include "Book.h"
#include "User.h"
#include "Account.h"
#include "BorrowRecord.h"
#include "Reservation.h"

using namespace std;

class Library {
private:
    vector<Book> books;
    vector<User *> users;
    vector<Account> accounts;
    vector<BorrowRecord> borrowRecords;
    vector<Reservation> reservations;

    void backupFile(const string &filename);
    void restoreFromBackup(const string &filename);
    bool isBookBorrowedByUser(const string &userID, const string &bookID) const;
    void initializeDefaultData();

public:
    ~Library();
    void loadBooks(const string &filename);
    void saveBooks(const string &filename);
    void loadUsers(const string &filename);
    void saveUsers(const string &filename);
    void loadAccounts(const string &filename);
    void saveAccounts(const string &filename);
    void loadBorrowRecords(const string &filename);
    void saveBorrowRecords(const string &filename);
    void loadReservations(const string &filename);
    void saveReservations(const string &filename);
    User *authenticate(const string &userID, const string &password);
    Account *findAccount(const string &userID);
    Book *findBook(const string &bookID) const;
    void displayBooks() const;
    vector<Book> searchBooks(const string &query) const;
    bool reserveBook(const string &userID, const string &bookID);
    void cancelReservation(const string &userID, const string &bookID);
    void displayReservations(const string &userID) const;
    vector<Book> &getBooks() { return books; }
    vector<User *> &getUsers() { return users; }
    vector<Account> &getAccounts() { return accounts; }
    vector<BorrowRecord> &getBorrowRecords() { return borrowRecords; }
    vector<Reservation> &getReservations() { return reservations; }
};

#endif // LIBRARY_H