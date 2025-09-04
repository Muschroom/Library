#pragma once
#include <string>

using namespace std;

class Book {
private:
	string title;
	string author;
	string isbn;
	string genre;
	bool available;
public:
	Book() : title(""), author(""), isbn(""), genre(""), available(true) {}

	Book(const string& title, const string& author, const string& isbn, const string& genre) : title(title), author(author), isbn(isbn), genre(genre), available(true) {}

	Book(Book* book): title(book->getTitle()), author(book->getAuthor()), isbn(book->getISBN()), genre(book->getGenre()), available(book->isAvailable()) {}
	
	string getTitle() {
		return this->title;
	}

	string getAuthor() {
		return this->author;
	}

	string getISBN() {
		return this->isbn;
	}

	string getGenre() {
		return this->genre;
	}

	bool isAvailable() {
		return this->available;
	}

	void setAvailable(bool available) {
		this->available = available;
	}

	~Book() = default;
};