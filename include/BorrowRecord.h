#ifndef BORROW_RECORD_H
#define BORROW_RECORD_H

#include <string>
#include "TimeUtils.h"

using namespace std;

class BorrowRecord {
private:
    string userID, bookID, ISBN;
    int borrowMinutes, borrowSeconds;
    int dueMinutes, dueSeconds;

public:
    BorrowRecord(string uid = "", string bid = "", string isbn = "", int bm = 0, int bs = 0, int dm = 0, int ds = 0);
    string getUserID() const { return userID; }
    string getBookID() const { return bookID; }
    string getISBN() const { return ISBN; }
    int getBorrowMinutes() const { return borrowMinutes; }
    int getBorrowSeconds() const { return borrowSeconds; }
    int getDueMinutes() const { return dueMinutes; }
    int getDueSeconds() const { return dueSeconds; }
    bool isOverdue() const;
    int calculateOverdueDays() const;
    string toCSV() const;
};

#endif // BORROW_RECORD_H