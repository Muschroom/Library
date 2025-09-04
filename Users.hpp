#pragma once
#include <string>
#include <unordered_set>
using namespace std;

class User {
private:
	string name;
	string email;
	string id;
	unordered_set<string>* borrowedbooks;
public:
	User() : name(""), email(""), id(""), borrowedbooks(new unordered_set<string>) {}

	User(const string& name, const string& email, const string& id) : name(name), email(email), id(id), borrowedbooks(new unordered_set<string>) {}

	User(User* user): name(user->getName()), email(user->getEmail()), id(user->getId()){
		borrowedbooks = new unordered_set<string>(*user->getBorrowedBooks());
	}

	string getName(){
		return this->name;
	}

	string getEmail(){
		return email;
	}

	string getId(){
		return id;
	}

	unordered_set<string>* getBorrowedBooks() {
		return this->borrowedbooks;
	}

	void setName(string& name) {
		this->name = name;
	}

	void setEmail(string& email) {
		this->email = email;
	}

	void setId(string id) {
		this->id = id;
	}

	bool canBorrow() {
		return this->getBorrowedBooks()->size() < this->getMaxBooks();
	}


	virtual int getMaxBooks() = 0;
	virtual int getMaxDays() = 0;
	virtual double getFinePerDay() = 0;

	~User() { delete borrowedbooks; }

public:


};

class Student : public User {
public:
	Student(const string& name, const string& email, const string& id) : User(name, email, id) {}

	int getMaxBooks() override {
		return 3;
	}

	int getMaxDays() override {
		return 14;
	}

	double getFinePerDay() override {
		return 0.5;
	}

	~Student() = default;
};

class Faculty : public User {
public:
	Faculty(const string& name, const string& email, const string& id) : User(name, email, id) {}

	int getMaxBooks() override {
		return 10;
	}

	int getMaxDays() override {
		return 30;
	}

	double getFinePerDay() override {
		return 15.5;
	}

	~Faculty() = default;
};


class Guest : public User {
public:
	Guest(const string& name, const string& email, const string& id) : User(name, email, id) {}

	int getMaxBooks() override {
		return 1;
	}

	int getMaxDays() override {
		return 7;
	}

	double getFinePerDay() override {
		return 1.5;
	}

	~Guest() = default;
};