#include "Savings.h"


Account* Account::Create()
{
	return new Savings();
}

void Savings::deposit(double amount)
{
	if (amount >= minAmountforInterest)
	{
		amount += amount * InterestRate;
	}
	Account::deposit(amount);
}

void Savings::withdraw(double amount)
{
	amount *= (1 + WithdrawPenalty);
	Account::withdraw(amount);
}

void Savings::print(std::ostream& os) const
{
	os<<" Savings Interest Rate: "<< InterestRate<< ", min Amount: " << minAmountforInterest
		<<", Saving Balance: "<<AccountBalance<<'\n';
		
}
