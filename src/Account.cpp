#include "Account.h"
#include <algorithm>

Account::Account(string id, string r, int cb, double fa)
    : userID(id), role(r), currentBorrowed(cb), fineAmount(fa) {}

void Account::addBorrowedBook(const string &bookID) {
    borrowHistory.push_back(bookID);
    currentBorrowed++;
}

void Account::removeBorrowedBook(const string &bookID) {
    auto it = find(borrowHistory.begin(), borrowHistory.end(), bookID);
    if (it != borrowHistory.end()) {
        borrowHistory.erase(it);
        currentBorrowed--;
    }
}

void Account::calculateFines(int daysOverdue, int fineRate) {
    fineAmount += max(0, daysOverdue) * fineRate;
}

void Account::payFines() {
    if (fineAmount > 0) {
        char confirm;
        cout << "Total fines: " << fineAmount << " rupees. Confirm payment (y/n)? ";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y') {
            fineAmount = 0;
            cout << "Fines paid successfully.\n";
        } else {
            cout << "Payment cancelled.\n";
        }
    } else {
        cout << "No fines to pay.\n";
    }
}

void Account::display() const {
    cout << "UserID: " << userID << ", Role: " << role << ", Current Borrowed: " << currentBorrowed
         << ", Fine Amount: " << fineAmount << endl;
    cout << "Borrowing History: ";
    if (borrowHistory.empty()) cout << "None";
    else for (const auto &book : borrowHistory) cout << book << " ";
    cout << endl;
}

string Account::toCSV() const {
    string history;
    for (const auto &book : borrowHistory) history += (history.empty() ? "" : " ") + book;
    return userID + "," + role + "," + to_string(currentBorrowed) + "," + to_string(fineAmount) + "," + history;
}