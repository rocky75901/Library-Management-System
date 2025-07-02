#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Account {
private:
    string userID, role;
    int currentBorrowed;
    double fineAmount;
    vector<string> borrowHistory;

public:
    Account(string id = "", string r = "", int cb = 0, double fa = 0);
    void addBorrowedBook(const string &bookID);
    void removeBorrowedBook(const string &bookID);
    void calculateFines(int daysOverdue, int fineRate);
    void payFines();
    void display() const;
    string getUserID() const { return userID; }
    string getRole() const { return role; }
    int getCurrentBorrowed() const { return currentBorrowed; }
    double getFineAmount() const { return fineAmount; }
    const vector<string> &getBorrowHistory() const { return borrowHistory; }
    void setFineAmount(double fa) { fineAmount = fa; }
    string toCSV() const;
};

#endif // ACCOUNT_H