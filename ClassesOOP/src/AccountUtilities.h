#pragma once

#include <vector>
#include "Account.h"

void display(std::vector<Account*>& accounts)
{
	for (const auto& acc : accounts)
	{
		std::cout << *acc << '\n';
	}
}

void withdraw(std::vector<Account*>& accounts, double amount)
{
	for (const auto& acc : accounts)
	{
		acc->withdraw(amount);
	}
}

void deposit(std::vector<Account*>& accounts, double amount)
{
	for (const auto& acc : accounts)
	{
		acc->deposit(amount);
	}
}