#pragma once
#include "Library.hpp"
#include <iostream>
using namespace std;

class Console {
private:
	Library lib;

private:
    int getInteger(const string& prompt) {
        int num;
        while (true) {
            cout << prompt;
            cin >> num;
            if (cin.fail()) {
                cout << "Enter number again\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.clear();
            }
            else {
                cout << "\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return num;
            }
        }
    }

    string getString(const string& prompt) {
        string str;
        cout << prompt;
        getline(cin, str);
        return str;
    }

private:
    void addBook() {
        string title = getString("Enter title:");
        string author = getString("Enter author:");
        string isbn = getString("Enter isbn:");
        string genre = getString("Enter genre:");

        if (lib.addBook(title, author, isbn, genre)) {
            cout << "Book was added\n";
        }
        else {
            cout << "Such book with similar isbn is already in library \n";
        }
        cout << "\n";
    }

    void removeBook() {
        string isbn = getString("Enter isbn:");

        if (lib.removeBook(isbn)) {
            cout << "Book was removed\n";
        }
        else {
            cout << "Our library does not have such a book or someone is already reading it\n";
        }
        cout << "\n";
    }

    void searchBooks() {
        string prompt = getString("Enter prompt:");
        vector<Book>* books = lib.searchBooks(prompt);
        if (books->size() == 0) {
            cout << "Our library does not have such books\n";
            return;
        }

        cout << "Amount of books: " << books->size() << "\n";
        for (auto book : *books) {
            cout <<"title: " << book.getTitle() << "\n";
            cout <<"author: " << book.getAuthor() << "\n";
            cout <<"isbn: " << book.getISBN() << "\n";
            cout <<"genre: " << book.getGenre() << "\n";
            cout <<"available 0|1 : " << book.isAvailable() << "\n";
            cout << "\n";
        }

        delete books;
    }

    void showBooks() {
        vector<Book>* books = lib.showBooks();
        if (books->size() == 0) {
            cout << "Our library is empty. Come later\n";
            return;
        }

        cout << "Amount of books: " << books->size() << "\n";
        for (auto book : *books) {
            cout <<"Title: " << book.getTitle() << "\n";
            cout <<"Author: " << book.getAuthor() << "\n";
            cout <<"ISBN: " << book.getISBN() << "\n";
            cout <<"Genre: " << book.getGenre() << "\n";
            cout << "\n";
        }

        delete books;
    }

private:
    void addUser() {
        string name = getString("Enter name:");
        string email = getString("Enter email:");
        string id = getString("Enter id:");

        cout << "Choose type of user\n";
        cout << "1. Student\n";
        cout << "2. Faculty\n";
        cout << "3. Guest\n";

        int choice = getInteger("Your choice: ");

        switch (choice)
        {
        case 1:
            if (lib.addUser(name, email, id, STUDENT)) {
                cout << "Student was added\n";
            }
            else {
                cout << "Such user with similar alredy id exists\n";
            }
            break;
        case 2:
            if (lib.addUser(name, email, id, FACULTY)) {
                cout << "Faculty was added\n";
            }
            else {
                cout << "Such user with similar alredy id exists\n";
            }
            break;
        case 3:
            if (lib.addUser(name, email, id, GUEST)) {
                cout << "Guest was added\n";
            }
            else {
                cout << "Such user with similar id alredy exists\n";
            }
            break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }

    void removeUser() {
        string id = getString("Enter id:");

        if (lib.removeUser(id)) {
            cout << "User was removed\n";
        }
        else {
            cout << "Our library does not have such users or user has not returned books yes\n";
        }
        cout << "\n";
    }

    void showUsers() {
        vector<User*>* users = lib.showUsers();
        if (users->size() == 0) {
            cout << "We have no customers\n";
            return;
        }

        cout << "Amount of users: " << users->size() << "\n";
        for (auto user : *users) {
            cout <<"Name: " << user->getName() << "\n";
            cout <<"Email: " << user->getEmail() << "\n";
            cout <<"Id: " << user->getId() << "\n";
            cout << "Borrowed books: " << user->getBorrowedBooks()->size() << "\n";
            cout << "\n";
        }

        delete users;
    }
private:
    void borrowBook() {
        string id = getString("Enter id:");
        string isbn = getString("Enter isbn:");

        if (lib.borrowBook(id, isbn)) {
            cout << "Book was borrowed\n";
        }
        else {
            cout << "Such book or user does not exists\n or book has been alredy reading\n or user has reached the limit\n";
        }
    }

    void returnBook() {
        string id = getString("Enter id:");
        string isbn = getString("Enter isbn:");

        if (lib.returnBook(id, isbn)) {
            cout << "Book was returned\n";
        }
        else {
            cout << "Such book or user does not exists\n or user has not borrowed such a book\n";
        }
    }

    void getDebts() {
        vector<Debt>* debts = lib.getDebts();
        if (debts->size() == 0) {
            cout << "No debts! We can not make money\n";
        }

        cout << "Amount of debts: " << debts->size() << "\n";
        for (auto debt : *debts) {
            cout << "User id: " << debt.userId << "\n";
            cout << "Book id: " << debt.bookId << "\n";
            cout << "Debt: " << debt.fee << " gold" << "\n";
        }

        delete debts;
    }

    void getRecords() {
        vector<Record>* records = lib.getRecords();
        if (records->size() == 0) {
            cout << "No records has been created yet\n";
        }

        cout << "Amount of records: " << records->size() << "\n";
        for (auto record : *records) {
            cout <<"User id: " << record.getUserId() << "\n";
            cout <<"Book id: " << record.getBookId() << "\n";
            cout <<"Return date: " << record.getDate() << "\n";
        }

        delete records;
    }

    void getOverdueBooks() {
        vector<Book>* overdues = lib.getOverdueBooks();
        if (overdues->size() == 0) {
            cout << "No overdues has been created yet\n";
        }

        cout << "Amount of overdues: " << overdues->size() << "\n";
        for (auto overdue : *overdues) {
            cout <<"Title: " << overdue.getTitle() << "\n";
            cout <<"Author: " << overdue.getAuthor() << "\n";
            cout <<"ISBN: " << overdue.getISBN() << "\n";
            cout <<"Genre: "<< overdue.getGenre() << "\n";
            cout << "\n";
        }

        delete overdues;
    }
private:
    void minusDays() {
        int days = max(0, getInteger("Enter number:"));

        if (lib.changeDates(days)) {
            cout << "Dates has been changed\n";
        }
        else {
            cout << "We have no records\n";
        }
    }

private:
	void showMenu() {
        cout << "Library interface\n";
        cout << "1. Work with books\n";
        cout << "2. Work with users\n";
        cout << "3. Work with borrowings\n";
        cout << "4. Use test menu\n";
	}

    void handleBooks() {
        cout << "Operations\n";
        cout << "1: Add a book\n";
        cout << "2: Remove a book\n";
        cout << "3: Search books\n";
        cout << "4: Show books\n";

        int choice = getInteger("Your choice: ");


        switch (choice) {
        case 1:
            addBook(); break;
        case 2:
            removeBook(); break;
        case 3:
            searchBooks(); break;
        case 4:
            showBooks(); break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }
    
    void handleUsers() {
        cout << "Operations\n";
        cout << "1: Add a user\n";
        cout << "2: Remove a user\n";
        cout << "3: Show users\n";

        int choice = getInteger("Your choice: ");


        switch (choice) {
        case 1:
            addUser(); break;
        case 2:
            removeUser(); break;
        case 3:
            showUsers(); break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }

    void handleBorrowings() {
        cout << "Operations\n";
        cout << "1: Borrow a book\n";
        cout << "2: Return a book\n";
        cout << "3: Show debts\n";
        cout << "4: Show records\n";
        cout << "5: Show overdue books\n";

        int choice = getInteger("Your choice: ");


        switch (choice) {
        case 1:
            borrowBook(); break;
        case 2:
            returnBook(); break;
        case 3:
            getDebts(); break;
        case 4:
            getRecords(); break;
        case 5:
            getOverdueBooks(); break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }

    void handleTests() {
        cout << "Commands\n";
        cout << "1. Minus days\n";

        int choice = getInteger("Your choice: ");

        switch (choice) {
        case 1:
            minusDays(); break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }

public:
	void run() {
        while (true) {
            showMenu();

            int choice = getInteger("Your choice: ");
            switch (choice) {
                case 1:
                    handleBooks(); break;
                case 2:
                    handleUsers(); break;
                case 3:
                    handleBorrowings(); break;
                case 4:
                    handleTests(); break;
                case 0:
                    return;
                default:
                    cout << "Invalid choice";
                    break;
                }
            }
        }

};