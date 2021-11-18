#pragma once

#include <iostream>
#include "Printable.h"


class Account:public IPrintable
{
public:
	virtual void deposit(double amount) = 0; 
	virtual void withdraw(double amount) = 0;

	double GetBalance() const { return AccountBalance; }
	
	virtual void print(std::ostream& os) const override
	{
		os << "Base Account Balance: " <<AccountBalance<<'\n';
	}
	
	virtual ~Account() {};

	static Account* Create();
protected:
	double AccountBalance=0.0;
};




