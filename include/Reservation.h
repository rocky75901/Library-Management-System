#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

using namespace std;

class Reservation {
private:
    string userID, bookID;
    int reservationMinutes, reservationSeconds;
    bool active;

public:
    Reservation(string uid = "", string bid = "", int rm = 0, int rs = 0, bool a = true);
    string getUserID() const { return userID; }
    string getBookID() const { return bookID; }
    bool isActive() const { return active; }
    void setActive(bool status) { active = status; }
    string toCSV() const;
};

#endif // RESERVATION_H