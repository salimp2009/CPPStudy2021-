#pragma once

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <exception>

/* Basic example for user defined exception class; Prefer Asserts*/
class DivisionByZeroException
{
public:
	DivisionByZeroException(const char* msg):m_message{msg} {}
	DivisionByZeroException(std::string msg) :m_message{ std::move(msg) } {}

	const std::string& GetMsg()const & { return m_message; };
	std::string GetMsg() && { return std::move(m_message); };

private:
	std::string m_message;
};

class NegativeValueException
{

};

void DivideByZero(double sum, double total)
{
	double average{ 0.0 };
	if( total==0 )throw DivisionByZeroException("division by zero");
	if (sum < 0 || total < 0) throw NegativeValueException();
	average = sum / total;

}


