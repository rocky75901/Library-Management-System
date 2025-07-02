#include "Reservation.h"

Reservation::Reservation(string uid, string bid, int rm, int rs, bool a)
    : userID(uid), bookID(bid), reservationMinutes(rm), reservationSeconds(rs), active(a) {}

string Reservation::toCSV() const {
    return userID + "," + bookID + "," + to_string(reservationMinutes) + "," +
           to_string(reservationSeconds) + "," + (active ? "1" : "0");
}