#pragma once
#include "Users.hpp"
#include "Book.hpp"
#include <unordered_map>
#include <chrono>
#include <sstream>
#include <ctime>
#include <list>
enum userType {
	STUDENT,
	FACULTY,
	GUEST
};

struct Debt {
	string userId;
	string bookId;
	double fee;
};


class Record {
protected:
	string userId;
	string bookId;
	chrono::system_clock::time_point returnDate;

public:
	Record() : userId(""), bookId("") {}

	Record(const string& id, const string& isbn, int maxDays) : userId(id), bookId(isbn) {
		returnDate = chrono::system_clock::now() + chrono::hours(24 * maxDays);
	}

	Record(Record* record): userId(record->getUserId()), bookId(record->getBookId()){
		returnDate = record->getReturnDate();
	}

	string getUserId() {
		return this->userId;
	}

	string getBookId(){
		return this->bookId;
	}

	chrono::system_clock::time_point getReturnDate(){
		return returnDate;
	}

	string getDate() {
		time_t time = std::chrono::system_clock::to_time_t(returnDate);

		char buf[26];
		ctime_s(buf, sizeof(buf), &time);
		buf[24] = '\0';
		return buf;
	}

	int getOverdueDays() {
		auto now = chrono::system_clock::now();
		auto hours = std::chrono::duration_cast<std::chrono::hours>(now - returnDate).count();

		return max(0, hours / 24);
	}

	

	~Record() = default;
};

class LibraryOperations {
public:
	virtual bool addBook(const string& title,const string& author,const string& isbn,const string& genre) = 0;
	virtual bool removeBook(const string& isbn) = 0;
	virtual Book findBook(const string& isbn) = 0;
	virtual vector<Book>* searchBooks(const string& query) = 0;
	virtual vector<Book>* showBooks() = 0;


	virtual bool addUser(const string& name, const string& email, const string& id, userType type) = 0;
	virtual bool removeUser(const string& id) = 0;
	virtual User* findUser(const string& id) = 0;
	virtual vector<User*>* showUsers() = 0;

	virtual bool borrowBook(const string& id, const string& isbn) = 0;
	virtual bool returnBook(const string& id, const string& isbn) = 0;
	virtual vector<Debt>* getDebts() = 0;
	virtual vector<Book>* getOverdueBooks() = 0;
	virtual vector<Record>* getRecords() = 0;
};


class Library : public LibraryOperations {
private:
	unordered_map<string, Book>* books;
	unordered_map<string, User*>* users;
	list<Record>* records;
public:
	Library(): books(new unordered_map<string, Book>()), users(new unordered_map<string, User*>()), records(new list<Record>()){}

	~Library() {
		for (auto iter = users->begin(); iter != users->end(); iter++) {
			delete iter->second;
		}

		delete users;
		delete books;
	}

public:
	bool addBook(const string& title,const string& author,const string& isbn,const string& genre) override{
		if (books->find(isbn) != books->end()) {
			return false;
		}

		books->insert({isbn, Book(title, author, isbn, genre) });

		return true;
	}

	bool removeBook(const string& isbn) override{
		if (books->find(isbn) == books->end()) {
			return false;
		}

		if (!books->find(isbn)->second.isAvailable()) {
			return false;
		}


		books->erase(isbn);
		return true;
	}

	Book findBook(const string& isbn) override {
		if (books->find(isbn) == books->end()) {
			return Book();
		}

		return books->find(isbn)->second;
	}

	vector<Book>* searchBooks(const string& query) override {
		vector<Book>* newBooks = new vector<Book>();

		for (auto iter = books->begin(); iter != books->end(); iter++) {
			Book& book = iter->second;

			if (book.getISBN() == query) {
				newBooks->push_back(book);
				continue;
			}

			if (book.getGenre() == query) {
				newBooks->push_back(book);
				continue;
			}

			string title = book.getTitle();
			if (title.find(query) != string::npos) {
				newBooks->push_back(book);
				continue;
			}

			string author = book.getAuthor();
			if (author.find(query) != string::npos) {
				newBooks->push_back(book);
			}

		}

		return newBooks;
	}

	vector<Book>* showBooks() override {
		vector<Book>* newBooks = new vector<Book>();

		for (auto iter = books->begin(); iter != books->end(); iter++) {
			newBooks->push_back(iter->second);
		}

		return newBooks;
	}

public:
	bool addUser(const string& name, const string& email, const string& id, userType type ) override {
		if (users->find(id) != users->end()) {
			return false;
		}

		User *user;
		switch (type) {
			case(STUDENT):
				user = new Student(name, email, id);
				break;
			case(FACULTY):
				user = new Faculty(name, email, id);
				break;
			case(GUEST):
				user = new Guest(name, email, id);
				break;
			default:
				return false;
		}

		users->insert({ id, user });
		return true;
	}

	bool removeUser(const string& id) override {
		if (users->find(id) == users->end()) {
			return false;
		}

		if (users->find(id)->second->getBorrowedBooks()->size() > 0) {
			return false;
		}

		delete users->find(id)->second;
		users->erase(id);

		return true;
	}

	User* findUser(const string& id) override {
		if (users->find(id) == users->end()) {
			return NULL;
		}

		return users->find(id)->second;
	}

	vector<User*>* showUsers() override {
		vector<User*>* newUseres = new vector<User*>();

		for (auto iter = users->begin(); iter != users->end(); iter++) {
			newUseres->push_back(iter->second);
		}

		return newUseres;
	}

public:
	bool borrowBook(const string& id, const string& isbn) override {
		if (users->find(id) == users->end() || books->find(isbn) == books->end()) {
			return false;
		}

		User* user = users->find(id)->second;
		Book& book = books->find(isbn)->second;

		if (!user->canBorrow() || !book.isAvailable()) {
			return false;
		}

		book.setAvailable(false);
		user->getBorrowedBooks()->insert(isbn);
		records->push_back(Record(user->getId(), book.getISBN(), user->getMaxDays()));

		return true;
	}

	bool returnBook(const string& id, const string& isbn) override {
		if (users->find(id) == users->end() || books->find(isbn) == books->end()) {
			return false;
		}

		User* user = users->find(id)->second;
		Book& book = books->find(isbn)->second;

		if (user->getBorrowedBooks()->find(isbn) == user->getBorrowedBooks()->end()) {
			return false;
		}

		book.setAvailable(true);
		user->getBorrowedBooks()->erase(isbn);

		records->remove_if([&](Record& record) {
			return record.getBookId() == isbn && record.getUserId() == id;
			});

		return true;
	}

	vector<Debt>* getDebts() override{
		vector<Debt>* debts = new vector<Debt>();
		
		for (auto iter = records->begin(); iter != records->end(); iter++) {
			if (iter->getOverdueDays() > 0) {
				double fee = users->find(iter->getUserId())->second->getFinePerDay();
				debts->push_back({ iter->getUserId(), iter->getBookId(), iter->getOverdueDays() * fee });
			}
		}

		return debts;
	}

	vector<Record>* getRecords() override{
		vector<Record>* newRecords = new vector<Record>();

		for (auto iter = records->begin(); iter != records->end(); iter++) {
			newRecords->push_back(*iter);
		}

		return newRecords;
	}

	vector<Book>* getOverdueBooks() override {
		vector<Book>* newBooks = new vector<Book>();

		for (auto iter = records->begin(); iter != records->end(); iter++) {
			if (iter->getOverdueDays() > 0) {
				newBooks->push_back(books->find(iter->getBookId())->second);
			}
		}

		return newBooks;
	}

public:
	//эта функция нужна только для тестирования getDebts, getOverdueBooks
	bool changeDates(int days) {
		if (records->size() == 0) {
			return 0;
		}

		list<Record>* newRecords = new list<Record>();

		for (auto &record : *records) {
			auto now = chrono::system_clock::now();
			auto returnDate = record.getReturnDate();
			auto hours = chrono::duration_cast<chrono::hours>(returnDate - now).count();

			newRecords->push_back(Record(record.getUserId(), record.getBookId(), - (days) + hours / 24));
		}

		delete records;
		records = newRecords;
		return true;
	}

};

