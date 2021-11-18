#pragma once
#include "Account.h"



class Savings: public Account
{
	static constexpr double InterestRate = 0.15;
	static constexpr double minAmountforInterest = 500.0;
	static constexpr double WithdrawPenalty = 0.05;
public:
	virtual void deposit(double amount) override;
	virtual void withdraw(double amount) override;
	virtual void print(std::ostream& os) const override;
	
	virtual ~Savings(){}
private:
	
};

