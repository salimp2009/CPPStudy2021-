#pragma once
#include "STLpch.h"

//#include <iostream>
//#include <cstdio>
//#include <string>
//#include <memory>
//#include <vector>


class Person
{
public:
std::string name;
std::shared_ptr<Person> mother;
std::shared_ptr<Person>father;

/*TODO: change to weak_ptr otherwise it creates cyclic reference and object dont get deleted when out of scope */
//std::vector<std::shared_ptr<Person>> kids;

std::vector<std::weak_ptr<Person>> kids;

Person(std::string n, std::shared_ptr<Person>m=nullptr, std::shared_ptr<Person> f=nullptr)
	:name{std::move(n)}, mother{m}, father{f} { }

~Person() { std::cout << "Deleting; " << name << '\n'; }

};


std::shared_ptr<Person> InitFamily(std::string name)
{
	std::shared_ptr<Person> mom(new Person{name + "'s mom"});
	std::shared_ptr<Person> dad(new Person{name + "'s dad"});
	std::shared_ptr<Person> kid(new Person{name, mom, dad });
	mom->kids.push_back(kid);
	dad->kids.push_back(kid);
	return kid;
}