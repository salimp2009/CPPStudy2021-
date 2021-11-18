#include "GameEnginePCH.h"

#include "Assertions.h"
#include "EndianSwap.h"
#include "BranchPrediction.h"
#include "ThreadGuard.h"
#include "ScopedThread.h"
#include "ReentrantLock.h"
#include "UnnecessaryLock.h"
#include "SIMDExamples.h"


int main()
{
	//ASSERT(3 == 5); 
	std::printf("Test Game Engine BOOK TEST\n");

	std::cout <<std::bitset<32>(255)<< '\n';

	/* Integer Endian SWAP EXAMPLE*/
	Example myExample;

	myExample.m_a = 5;
	myExample.m_b = 2;
	myExample.m_c = 1;

	std::cout << "m_a : " << std::bitset<32>(myExample.m_a)<<'\n';

	myExample.m_a = SwapInt32(myExample.m_a);
	std::cout << "m_a (afterswaped to big endian) : " << std::bitset<32>(myExample.m_a) << '\n';

	/* example for float swapping*/
	U32F32 testval;
	testval.m_asF32 = 35.4f;
	std::cout << "testswap (float) before swapped: " << std::bitset<32>(testval.m_asU32) << '\n';

	testval.m_asF32 = swapF32(testval.m_asF32);
	std::cout << "testswap (float) AFTER swapped: " << std::bitset<32>(testval.m_asU32) << '\n';

	float mynum = 135.4f;
	mynum=swapF32(mynum);

	SafeIntegerDivide(10, 5, 0);
	/*
		Can also be used for integers; if the return type casted; prefer Branch prediction off compilers !!!
	*/
	std::cout << SafeFloatDivide_pred(10.0f, 3.0f, 2.0f) << '\n';

	//std::atomic<int> localState{ 0 };

	/* preferabbly do not pass variables/or shared data by ref cause if it changes in another thread
		you might get unexpected result unless it is inteded to do so
	*/

	//UnnecessaryLock g_Lock;

	//auto myfunc = [&localState, &g_Lock] () noexcept
	//{
	//	/* assert macros test if the critical sectioon overlapp or not; this is an example to test
	//	   if a lock or any sync function is needed for any given critical operation
	//	*/
	//	//BEGIN_ASSERT_LOCK_NOT_NECESSARY(g_Lock);
	//	ASSERT_LOCK_NOT_NECESSARY(janitor, g_Lock);
	//	std::printf("local state = %i\n", ++localState);
	//	fmt::print("thread id {}\n", std::this_thread::get_id());
	//	//END_ASSERT_LOCK_NOT_NECESSARY(g_Lock);
	//};

	//std::thread th1{myfunc};
	
	//threadguard thguard{ th1 };
		
	//scopedThread t{ std::thread{myfunc} };
	//localState.store(15);

	std::printf("Hello from main thread\n");

	TestAddSSE();
	TestDotArraySSE();
}