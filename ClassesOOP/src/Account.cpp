#include "Account.h"

//void Account::print(std::ostream& os) const
//{
//	 os << "Account is IPrintable" << '\n'; 
//}

void Account::deposit(double amount)
{
	if (amount < 0)
	{
		printf("Negative amounts are Invalid!! Try again!!!\n");
		return;
	}
		
	AccountBalance+=amount;
	printf("Deposit successfull!!! Account Balance: %0.2f\n", AccountBalance);
}

void Account::withdraw(double amount)
{
	if (AccountBalance - amount < 0)
	{
		printf("Accounts Balance is not Enough for Withdraw\n");
		return;
	}
	AccountBalance -= amount;
	printf("WithDraw successfull!!! Account Balance: %0.2f \n", AccountBalance);
}
