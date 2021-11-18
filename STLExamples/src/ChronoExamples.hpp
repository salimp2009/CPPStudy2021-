#pragma once

template<typename C>
void printClockData()
{
	/*period is the unit type in seconds of a given clock type*/
	typedef typename C::period  P;
	if (std::ratio_less_equal<P, std::milli>::value)
	{
		/*convert to millisecs and print*/
		typedef typename std::ratio_multiply<P, std::kilo>::type TT;
		std::cout << std::fixed << double(TT::num) / TT::den << " milliseconds \n";
	}

	else
	{
		std::cout << std::fixed << double(P::num) / P::den << " seconds \n";
	}

	/*is_steady; is a boolean value if the clock is steady*/
	std::cout << "- is steady: " << std::boolalpha << C::is_steady << '\n';

}


void Chrono_Test()
{
	std::printf("\n---------------------Chrono Examples------------------------------------\n\n");

	std::chrono::duration<int>							twentySecs(20);
	std::chrono::duration<double, std::ratio<60>>		halfAMinute(0.5);
	std::chrono::duration<double, std::ratio<1, 1000>>	oneMillisecond(1);

	std::cout << "20 secs: " << twentySecs.count() << '\n';
	std::cout << "30 secs (Half a minute) : " << halfAMinute.count() << '\n';

	/*Chrono library built in definitions*/
	std::chrono::seconds		tenSecs(10);
	std::chrono::minutes		oneMinute(1);
	std::chrono::milliseconds	fiveMillisecs(5);
	std::chrono::hours			oneDay(24);

	std::cout << "Five millisecs: " << fiveMillisecs.count() << '\n';

	auto newduration = tenSecs - oneMinute;

	std::cout << newduration.count() << '\n';
	std::cout << newduration.count() * (-5) << '\n';
	std::cout << (++newduration).count() << '\n';

	std::cout << newduration << '\n';

	std::chrono::seconds sec(55);
	std::chrono::minutes m1;

	m1 = std::chrono::duration_cast<std::chrono::minutes>(sec);

	std::cout << "\n-------steady clock---------------\n";
	printClockData<std::chrono::steady_clock>();

	std::cout << "\n-------high_res clock---------------\n";
	printClockData<std::chrono::high_resolution_clock>();

	std::cout << "\n-------system clock---------------\n";
	printClockData<std::chrono::high_resolution_clock>();

	std::printf("\n---------------------Chrono Examples------------------------------------\n\n");
}


