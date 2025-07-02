#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

using namespace std;

class Book {
private:
    string bookID, title, author, publisher, ISBN, status;
    int year;

public:
    Book(string id = "", string t = "", string a = "", string p = "", int y = 0, string isbn = "", string s = "Available");
    string getBookID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getPublisher() const { return publisher; }
    int getYear() const { return year; }
    string getISBN() const { return ISBN; }
    string getStatus() const { return status; }
    void setStatus(string newStatus) { status = newStatus; }
    void setTitle(string t) { title = t; }
    void setAuthor(string a) { author = a; }
    void setPublisher(string p) { publisher = p; }
    void setYear(int y) { year = y; }
    void setISBN(string isbn) { ISBN = isbn; }
    void display() const;
    string toCSV() const;
};

#endif // BOOK_H