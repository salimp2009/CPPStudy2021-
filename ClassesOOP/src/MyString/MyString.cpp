#include "MyString.h"
#include <cstring>
#include <cctype>
#include <array>
#include <iostream>
#include <istream>

MyString::MyString(): str{nullptr}
{
	str = new char[1];
	*str = '\0';
}

MyString::MyString(const char* s):str{nullptr}
{
	// this is need so we dont have check this for MyString
	if (s == nullptr)
	{
		str = new char[1];
		*str = '\0';
	}
	else
	{
		/** strlen does not in last zero termination*/
		str = new char[std::strlen(s) + 1];
		std::memcpy(str, s, std::strlen(s)+1);
		//printf("Copy constructor for const char* !!!! \n");
	}
}

MyString::MyString(const MyString& source):str{nullptr}
{
	str = new char[std::strlen(source.str) + 1];
	std::memcpy(str, source.str, std::strlen(source.str) + 1);
	//printf("Copy constructor !!!! \n");
}

MyString::MyString(MyString&& source) noexcept:str{source.str} 
{
	source.str = nullptr;
	//printf("Move constructor !!!! \n");
}

MyString::~MyString()
{
	//printf("Deleting : %s \n", (str != nullptr ? str : "nullptr"));
	delete[]str;
}


MyString& MyString::operator=(const MyString& source)
{
	//printf("Copy assignment for const MyString!!!\n");
	if (this == &source)
	{
		return *this;
	}
	
	delete[] str;
	str = new char[std::strlen(source.str) + 1];
	std::memcpy(str, source.str, std::strlen(source.str) + 1);
	return *this;
}

MyString& MyString::operator=(MyString&& source) noexcept
{
	printf("Move assignment for const MyString!!!\n");
	if (this == &source) return *this;
	delete[]str;
	str = source.str;
	source.str = nullptr;
	return *this;
}

// Upper case conversion;
MyString MyString::operator-() const
{
	char* buffer = new char[std::strlen(str) + 1];
	//memcpy(buffer, str, std::strlen(str) + 1);	// not needed since we making direct assignment below;
	for (size_t i = 0; i < std::strlen(str)+1; ++i)
	{
		buffer[i] = std::tolower(str[i]);
	}
	MyString temp{ buffer };
	delete[] buffer;
	return temp;
}

MyString MyString::operator+(const MyString& rhs) const
{
	char* buffer = new char[strlen(str) + strlen(rhs.str) + 1];
	memcpy(buffer, str, strlen(str));
	memcpy(buffer + strlen(str), rhs.str, strlen(rhs.str) + 1);
	MyString temp{buffer};
	delete[] buffer;
	return temp;
}

MyString& MyString::operator+=(const MyString& rhs)
{
	*this = *this + rhs;
	return *this;
}

bool MyString::operator==(const MyString& rhs) const
{
	return (std::strcmp(str, rhs.str)==0);
}

bool MyString::operator!=(const MyString& rhs) const
{
	return !(str==rhs.str);
}

/** "lhs<rhs" returns -1;
	"lsh==rhs" returns 0 ; 
	"lhs>rhs1" returns 1
*/
bool MyString::operator<(const MyString& rhs) const
{
	return (std::strcmp(str, rhs.str) < 0 );
}

bool MyString::operator>(const MyString& rhs) const
{
	return (std::strcmp(str, rhs.str) > 0 );
}

/* Preincrement; add itself to itself*/
MyString& MyString::operator++()
{
	*this += *this;
	return *this;
}

/* Post increment version of above*/
MyString MyString::operator++(int)
{
	MyString oldvalue = *this;
	*this += *this;
	return oldvalue;
}

std::ostream &operator<<(std::ostream& os, const MyString& obj)
{
	os << obj.str;
	return os;
}

/** cin >> buffer is not recognized in c++ laterst version ; works for c++17
 * CHAR* overload for std::cin is removed in c++20
*/
#if HG_20
std::istream& operator>>(std::istream& in, MyString& obj) {
	char* buffer=new char[1000];
   	in >> buffer;
	obj = MyString{ buffer };
	delete[] buffer;
	return in;
}
#endif



