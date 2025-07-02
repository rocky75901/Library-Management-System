#include "BorrowRecord.h"

BorrowRecord::BorrowRecord(string uid, string bid, string isbn, int bm, int bs, int dm, int ds)
    : userID(uid), bookID(bid), ISBN(isbn), borrowMinutes(bm), borrowSeconds(bs), dueMinutes(dm), dueSeconds(ds) {}

bool BorrowRecord::isOverdue() const {
    int currentMinutes, currentSeconds;
    getCurrentTime(currentMinutes, currentSeconds);
    int totalCurrentSeconds = currentMinutes * 60 + currentSeconds;
    int totalDueSeconds = dueMinutes * 60 + dueSeconds;
    return totalCurrentSeconds > totalDueSeconds;
}

int BorrowRecord::calculateOverdueDays() const {
    int currentMinutes, currentSeconds;
    getCurrentTime(currentMinutes, currentSeconds);
    int elapsedDays = convertToSimulatedDays(borrowMinutes, borrowSeconds, currentMinutes, currentSeconds);
    int dueDays = convertToSimulatedDays(borrowMinutes, borrowSeconds, dueMinutes, dueSeconds);
    return max(0, elapsedDays - dueDays);
}

string BorrowRecord::toCSV() const {
    return userID + "," + bookID + "," + ISBN + "," + to_string(borrowMinutes) + "," + to_string(borrowSeconds) + "," +
           to_string(dueMinutes) + "," + to_string(dueSeconds);
}