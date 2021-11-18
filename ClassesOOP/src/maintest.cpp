
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <memory>
#include <string>

#include "Account.h"
#include "Savings.h"
#include "Printable.h"
#include "AccountUtilities.h"
#include "MyString/MyString.h"
#include "ExceptionSamples.h"
#include "ReadTextFile.h"

template<typename T>
void do_withdraw(T&& account, double amount)
{
	account->withdraw(amount);
}

void Print_IPrintables(const IPrintable& obj)
{
	std::cout <<"Iprintable obj: "<<obj << '\n';
}


class Base
{

public:
	void display() { std::printf("Base Protected Function \n");}
	
protected:
	int a=0;

};

class Derived :public Base
{
protected:
	void drev_display(){ std::printf("Base Protected Function \n"); }
};

void TestClassProtected()
{

	//Base  b;
	//b.display();

	//Derived d;
	//d.display();

}

void TestMyString()
{
	
	MyString s1{ "SALIM" };
	s1.display();

	MyString temp = -s1;
	std::cout << "TEMP: ";
	temp.display();

	///* cannot use const char* on the very left side
	//since + operator is a member function expects left side to be MyString */
	//std::cout << "temp + s1 = ";
	//temp = temp + s1+ "Anything";  
	//temp.display();

	temp += s1;
	std::cout << "temp+=s1; ";
	temp.display();

	
	(temp++).display();
	temp.display();
	
	std::cout << "Hello my overloaded temp: " << ++temp << '\n';


/* TODO: Not works with c++latest ; works with c++17 
* write a BETTER macro to strip out if c++latest otherwise should work
*/
#if HG_20
	std::cout << "Please write a string for temp2: ";
	MyString temp2=new char[1000];
	std::cin >>temp2;
	std::cout << "Temp2: " << temp2 << '\n';
#endif

	int* n = new int();
	std::cin >>*n;
	std::cout << *n << '\n';
	//std::cout << std::boolalpha;
	//std::cout <<"s1=s1: "<< (s1 == s1) << '\n';
	//std::cout <<"Didem < Demir : "<< (MyString("Didem")<MyString("Didem")) << '\n';

	//MyString s2{ std::move(s1) };
	//s2.display();

	//MyString s3{ s2 };
	//s3.display();
	//std::cout <<"s3 length: "<<s3.getlength() << '\n';

	//s3 = "Didem";
	//s3.display();

	//s2 = s3;
	//s2.display();

	//s2 = nullptr;
	//s2.display();

}

void TestAccount()
{

	/** Testing a static function declared in base class, implemented in child class
	Creating pointer type of base class pointing to child class
	and calling the static function to return an instance of child class on Heap; Testing for Game Engine class
*/

	std::unique_ptr<Account> uptr1;
	uptr1.reset(Account::Create());
	uptr1->deposit(1000);
	std::cout << *uptr1 << '\n';

	//Savings t;
	//Account* ref1 = &t;
	//Account* ref2 = new Savings();

	//std::vector<Account*> AccntList{ ref1, ref2 };

	//display(AccntList);

	//withdraw(AccntList, 500);

	//deposit(AccntList, 2000);
	//display(AccntList);

	//withdraw(AccntList, 1200);
	//display(AccntList);


 //   ref1->deposit(2000);
 //   ref2->deposit(150);

	//std::cout << *ref1 << '\n';
	//std::cout << *ref2 << '\n';

	//ref1->withdraw(5000);
	//ref2->withdraw(200); 

	//Print_IPrintables(*ref1);
	//Print_IPrintables(*ref2);
	//
	//do_withdraw(std::forward<decltype(ref1)>(ref1), 1000);
	//do_withdraw(ref2, 200);

	//std::cout << *ref1 << '\n';
	//std::cout << *ref2 << '\n';

	//delete ref2;

}

void Test_CString()
{
	char* cptr = new char[4]{ 'a', 'b', 'c' };
	std::for_each(&cptr[0], &cptr[0] + std::strlen(cptr), [](const auto& c) {std::cout << c << '\n'; });

	std::cout << "cptr length : " << std::strlen(cptr) << '\n';

	//char str[] = "salim";
	//char* pstr = str;

	//char* pnew = new char[std::strlen(pstr)+1];
	//memcpy(pnew, pstr, std::strlen(pstr) + 1);

	//std::cout << pnew << '\n';
	//std::cout << std::strlen(pstr) << '\n';
	//std::cout << sizeof pnew << '\n';
	//std::cout << sizeof(int64_t)<< '\n';

	//printf("Deleting : %s", (pnew != nullptr ? pnew : "nullptr"));

	//delete[]pnew;

}

int main()
{

	//ReadFromFile();
	//ReadFromFile_Poem();
	StudentAnswers_Check();

	//TestMyString();

	//TestAccount();
	//Test_CString();
	

	//try
	//{
	//	DivideByZero(-200.0, 0.0);
	//}
	//catch (const DivisionByZeroException& e) {
	//	std::cout <<"Cant Divide by Zero: " <<e.GetMsg()<< '\n';
	//}
	//catch ([[maybe_unused]] const NegativeValueException& ex) {
	//	std::cout << "Negative values not excepted!!!"<< '\n';
	//}

	//std::cout << "program continues; from main \n";
}