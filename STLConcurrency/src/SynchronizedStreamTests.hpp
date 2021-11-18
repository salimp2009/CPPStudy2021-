#pragma once
#include "ConcurrencyPCH.h"

// Note: std::cout is thread safe ; the only issue happens with multiple threads interleaving of the characters
// interleaving can be prevent with other thread sync method methods as well

class Worker
{
public:
	Worker(std::string n) : name{ std::move(n) } {}
	void operator()()
	{
		for (auto i = 0; i < 3; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			//osyncstream uses a syncbuffer which uses mutex to sync to prevent interleaving but it has the cost of mutex
			std::osyncstream syncStream(std::cout);
			syncStream << name << " : " << "Work " << i << " done!!!\n";
		}
	}

private:
	std::string name;
};


inline void SynchronizedOutPut()
{
	std::printf("\n-SynchorinizedOutPut-\n");

	std::puts("Boss: Start Working !!!\n");

	std::jthread herb	= std::jthread(Worker("Herb"));
	std::jthread andrei	= std::jthread(Worker("  Andrei"));
	std::jthread scott	= std::jthread(Worker("    Scott"));
	std::jthread bjarne	= std::jthread(Worker("      Bjarne"));
	std::jthread bart	= std::jthread(Worker("         Bart"));
	std::jthread jenne	= std::jthread(Worker("            Jenne"));
	
}


