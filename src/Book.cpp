#include "Book.h"

Book::Book(string id, string t, string a, string p, int y, string isbn, string s)
    : bookID(id), title(t), author(a), publisher(p), year(y), ISBN(isbn), status(s) {}

void Book::display() const {
    cout << "BookID: " << bookID << ", Title: " << title << ", Author: " << author
         << ", Publisher: " << publisher << ", Year: " << year << ", ISBN: " << ISBN
         << ", Status: " << status << endl;
}

string Book::toCSV() const {
    return bookID + "," + title + "," + author + "," + publisher + "," + to_string(year) + "," + ISBN + "," + status;
}