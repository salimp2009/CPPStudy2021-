#pragma once
#include <string>
#include <iostream>

class MyString
{
	friend std::ostream& operator<<(std::ostream& os, const MyString& obj);
	
#if HG_20
	/* Does Not works with c++latest ; works with c++17*; char* overload in std::cin is removed in C++*/
	friend std::istream& operator>>(std::istream& in, MyString& obj);
#endif

private:
	char* str;
	
public:
	MyString();
	MyString(const char* s);
	MyString(const MyString& source);
	MyString(MyString&& source) noexcept;
	~MyString();

	MyString& operator=(const MyString& source)  ;
	MyString& operator=(MyString&& source) noexcept;

	// Lower case conversion
	MyString operator-() const;

	//Overloaded member functions
	MyString operator+(const MyString& rhs) const;
	MyString& operator+=(const MyString& rhs);

	bool operator==(const MyString& rhs) const;
	bool operator!=(const MyString& rhs) const;

	bool operator<(const MyString& rhs) const;
	bool operator>(const MyString& rhs) const;

	MyString& operator++();
	MyString operator++(int);

	void display() const { std::cout << "MyString: " << str << ", length: " << std::strlen(str) << '\n'; }
	size_t  getlength() const { return std::strlen(str); }

	const char* getstr() const { return str; }
	MyString GetMyString() { return *this; }

	// TODO ; test if this works as move !!!
	char* getstr() { return std::move(str); }

	// TODO; test if it works
	//const MyString& getstr() const& { return str; }
	//MyString&& getstr() && { return std::move(str); }

};






