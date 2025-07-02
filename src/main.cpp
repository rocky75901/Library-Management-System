#include "Library.h"
#include <iostream>
#include <limits>

using namespace std;

int main()
{
    Library library;

    library.loadBooks("books.csv");
    library.loadUsers("users.csv");
    library.loadAccounts("accounts.csv");
    library.loadBorrowRecords("borrowed_books.csv");
    library.loadReservations("reservations.csv");

    while (true)
    {
        cout << "\n=== Library Management System ===\n";
        cout << "Enter UserID (or type 'exit' to quit): ";
        string userID;
        cin >> userID;
        if (userID == "exit")
            break;

        cout << "Enter Password: ";
        string password;
        cin >> password;

        User *user = library.authenticate(userID, password);
        if (!user)
        {
            cout << "Authentication failed. Invalid UserID or Password.\n";
            continue;
        }

        Account *account = library.findAccount(userID);
        if (!account)
        {
            library.getAccounts().emplace_back(userID, user->getRole());
            account = library.findAccount(userID);
            library.saveAccounts("accounts.csv");
        }

        if (user->getRole() == "Student" || user->getRole() == "Faculty")
        {
            while (true)
            {
                cout << "\n=== " << user->getRole() << " Menu ===\n";
                cout << "1. Search Books\n2. Borrow Book\n3. Return Book\n4. View Account\n5. Pay Fines\n6. Reserve Book\n7. Cancel Reservation\n8. View Reservations\n9. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (cin.fail())
                {
                    cin.clear();                                         
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "Invalid choice. Please enter a number between 1 and 9.\n";
                    continue;
                }

                if (choice == 9)
                    break;

                switch (choice)
                {
                case 1:
                {
                    string searchQuery;
                    cout << "Enter search term (title, author, or ISBN): ";
                    cin.ignore();
                    getline(cin, searchQuery);
                    vector<Book> results = library.searchBooks(searchQuery);
                    if (results.empty())
                        cout << "No books found matching your search.\n";
                    else
                    {
                        cout << "\nSearch Results:\n";
                        for (const auto &book : results)
                            book.display();
                    }
                    break;
                }
                case 2:
                {
                    cout << "Available Books:\n";
                    library.displayBooks();
                    cout << "Enter BookID to borrow: ";
                    string bookID;
                    cin >> bookID;
                    Book *book = library.findBook(bookID);
                    if (book)
                    {
                        user->borrowBook(*book, *account, library.getBorrowRecords(), library.getReservations());
                        library.saveBooks("books.csv");
                        library.saveBorrowRecords("borrowed_books.csv");
                        library.saveAccounts("accounts.csv");
                        library.saveReservations("reservations.csv");
                    }
                    else
                        cout << "Book not found.\n";
                    break;
                }
                case 3:
                {
                    cout << "Enter BookID to return: ";
                    string bookID;
                    cin >> bookID;
                    Book *book = library.findBook(bookID);
                    if (book)
                    {
                        user->returnBook(*book, *account, library.getBorrowRecords(), library.getReservations());
                        library.saveBooks("books.csv");
                        library.saveBorrowRecords("borrowed_books.csv");
                        library.saveAccounts("accounts.csv");
                    }
                    else
                        cout << "Book not found.\n";
                    break;
                }
                case 4:
                    account->display();
                    break;
                case 5:
                    account->payFines();
                    library.saveAccounts("accounts.csv");
                    break;
                case 6:
                {
                    cout << "Available Books:\n";
                    library.displayBooks();
                    cout << "Enter BookID to reserve: ";
                    string bookID;
                    cin >> bookID;
                    library.reserveBook(userID, bookID);
                    library.saveBooks("books.csv");
                    library.saveReservations("reservations.csv");
                    break;
                }
                case 7:
                {
                    cout << "Enter BookID to cancel reservation: ";
                    string bookID;
                    cin >> bookID;
                    library.cancelReservation(userID, bookID);
                    library.saveBooks("books.csv");
                    library.saveReservations("reservations.csv");
                    break;
                }
                case 8:
                    library.displayReservations(userID);
                    break;
                default:
                    cout << "Invalid choice. Please enter a number between 1 and 9.\n";
                }
            }
        }
        else if (user->getRole() == "Librarian")
        {
            Librarian *librarian = dynamic_cast<Librarian *>(user);
            while (true)
            {
                cout << "\n=== Librarian Menu ===\n";
                cout << "1. Add Book\n2. Remove Book\n3. View Books\n"
                     << "4. Add User\n5. Remove User\n6. Search Books\n"
                     << "7. Logout\nChoice: ";
                int choice;
                cin >> choice;

                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid choice. Please enter a number between 1 and 7.\n";
                    continue;
                }

                if (choice == 7)
                    break;

                switch (choice)
                {
                case 1:
                {
                    string bookID, title, author, publisher, ISBN;
                    int year;
                    cout << "Enter BookID: ";
                    cin >> bookID;
                    cout << "Enter Title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter Author: ";
                    getline(cin, author);
                    cout << "Enter Publisher: ";
                    getline(cin, publisher);
                    cout << "Enter Year: ";
                    cin >> year;
                    cout << "Enter ISBN: ";
                    cin >> ISBN;
                    librarian->addBook(library.getBooks(), Book(bookID, title, author, publisher, year, ISBN));
                    library.saveBooks("books.csv");
                    break;
                }
                case 2:
                {
                    cout << "Enter BookID to remove: ";
                    string bookID;
                    cin >> bookID;
                    librarian->removeBook(library.getBooks(), bookID);
                    library.saveBooks("books.csv");
                    break;
                }
                case 3:
                    library.displayBooks();
                    break;
                case 4:
                {
                    string newUserID, name, role, password;
                    cout << "Enter UserID: ";
                    cin >> newUserID;
                    cout << "Enter Name: ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Enter Role (Student/Faculty/Librarian): ";
                    cin >> role;
                    cout << "Enter Password: ";
                    cin >> password;
                    librarian->addUser(library.getUsers(), library.getAccounts(), newUserID, name, role, password);
                    library.saveUsers("users.csv");
                    library.saveAccounts("accounts.csv");
                    break;
                }
                case 5:
                {
                    cout << "Enter UserID to remove: ";
                    string removeUserID;
                    cin >> removeUserID;
                    librarian->removeUser(library.getUsers(), library.getAccounts(), removeUserID);
                    library.saveUsers("users.csv");
                    library.saveAccounts("accounts.csv");
                    break;
                }
                case 6:
                {
                    string searchQuery;
                    cout << "Enter search term (title, author, or ISBN): ";
                    cin.ignore();
                    getline(cin, searchQuery);
                    vector<Book> results = library.searchBooks(searchQuery);
                    if (results.empty())
                        cout << "No books found matching your search.\n";
                    else
                    {
                        cout << "\nSearch Results:\n";
                        for (const auto &book : results)
                            book.display();
                    }
                    break;
                }
                default:
                    cout << "Invalid choice. Please enter a number between 1 and 7.\n";
                }
            }
        }
    }

    library.saveBooks("books.csv");
    library.saveUsers("users.csv");
    library.saveAccounts("accounts.csv");
    library.saveBorrowRecords("borrowed_books.csv");
    library.saveReservations("reservations.csv");

    cout << "Exiting Library Management System.\n";
    return 0;
}