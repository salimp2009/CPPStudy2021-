#pragma once
#include "RangesPCH.hpp"

#include "RangesContainerView.hpp"


inline void RangeExample01()
{
	std::printf("\n---RangeExample01---\n");

	const std::vector<int> numbers = { 1,2,3,4,5,6};

	auto result = numbers | std::views::filter([](int n) { return n % 2 == 0; })
						  | std::views::transform([](int n) { return n * 2; });
						  //| std::ranges::views::split(2);

	for (auto v : result)
	{
		fmt::print("{} ", v);
		
	}
	puts("\n");

	std::vector<int> vec2 = { -3,5,0,7,-4 };
	std::ranges::sort(vec2);
	for (const auto elem : vec2)
	{
		fmt::print("{} ", elem);
	}
	puts("\n");
}


struct PhoneBookEntry
{
	std::string name;
	int number = 0;
};

void PrintPhonebook(std::span<PhoneBookEntry> phoneBook)
{
	for (auto elem : phoneBook)
	{
		fmt::print("name: {}, number: {}\n", elem.name, elem.number);
	}
		
}


inline void RangeProjection()
{
	std::printf("\n--- RangeProjection---\n");

	std::vector<PhoneBookEntry> phonebook = { {"Salim", 222}, {"Didem", 568}, {"Taylor", 145}, {"Frank", 558} };

	puts("\nranges with default sort: less(); projection; name to sort");
	std::ranges::sort(phonebook, {}, &PhoneBookEntry::name);
	PrintPhonebook(phonebook);

	puts("\nranges with default sort: less(); projection; number to sort");
	std::ranges::sort(phonebook, {}, &PhoneBookEntry::number);
	PrintPhonebook(phonebook);

	puts("\nranges with default ranges::greater(); projection; number to sort");
	std::ranges::sort(phonebook, std::ranges::greater(), &PhoneBookEntry::number);
	PrintPhonebook(phonebook);

}

inline void ViewsKeysValue_Test()
{
	std::printf("\n--- Views:KeysValue---\n");

	std::unordered_map<std::string, int>freqWord = { {"witch", 25}, {"wizard", 33}, {"tale", 45}, {"dog", 4}, {"cat", 34}, {"fish", 23}};
	puts("Keys:");
	auto names = std::views::keys(freqWord);

	for (const auto& name : names)
	{
		fmt::print("{} ", name);
	}
	puts("\nValues: ");

	for (auto value : std::views::values(freqWord))
	{
		fmt::print("{} ", value);
	}
}

inline void ViewsKeys_FunctCompose()
{
	std::printf("\n--- Views:KeysValue---\n");

	std::map<std::string,int>freqWord2 = { {"witch", 25}, {"wizard", 33}, {"tale", 45}, {"dog", 4}, {"cat", 34}, {"fish", 23} };
	puts("\nWords:");
	for (const auto& name : std::views::keys(freqWord2))
	{
		fmt::print("{} ", name);
	}
	puts("\n\nAll Words Reverse Order: ");
	for (const auto& name : std::views::keys(freqWord2) | std::views::reverse)
	{
		fmt::print("{} ", name);
	}
	puts("\n\nThe First 4 Words: ");

	//TODO: MSVC Bug does not compile; give a narrowing error !!!; propose solution is on github/VS Developer site; 
	// https://developercommunity.visualstudio.com/t/Compile-error-with-std::ranges::views::k/1397309?from=email&viewtype=all&space=62&q=ranges+
	// https://github.com/microsoft/STL/pull/1844
	//for (const auto& name : std::views::keys(freqWord2) | std::views::take(4))
	//{
	//	fmt::print("{} ", name);
	//}

	puts("\n\nName Starts with 'w': ");

	auto firstLetterW = [](const std::string& name) { return name[0] == 'w'; };

	// use views::filter at the beginning of the pipe for the performance reason otherwise views::keys will be called twice for elements not filtered out
	for (const auto& name : std::views::keys(freqWord2) | std::views::filter(firstLetterW))
	{
		fmt::print("{} ", name);
	}

	puts("\nend ofName Starts with 'w': ");
	
}

inline void ViewsIota_FunctCompose()
{
	std::printf("\n--- ViewsIota_FunctCompose---\n");

	std::vector<int> vec1;
	std::vector<int> vec2;

	for (auto i : std::views::iota(0, 10))
	{
		vec1.push_back(i);
	}
	
	// when pass only one arg to iota it creates only one argumentand will generate more when we ask/apply views::take we get only 10 of it
	// This is an example of lazy evaluation ; iota will generate only one at the beginning and will do more when asked for more!
	for (auto i : std::views::iota(0) | std::views::take(10))
	{
		vec2.push_back(i);
	}

	fmt::print("vec1: {}\n", fmt::join(vec1, ", "));
	fmt::print("vec2: {}\n", fmt::join(vec2, ", "));
}


inline void ViewsIota_PrimeNumbers()
{
	std::printf("\n--- ViewsIota_PrimeNumbers---\n");


	puts("Number from 1'000'000 to 1'001'000 displayed each 100th");

	for (std::size_t i : std::views::iota(1'000'000u, 1'001'000u))
	{
		if(i%100==0)
		{ 
			std::printf("%zu ", i);
		}
	}

	puts("\n");

	auto odd = [](int j) { return j % 2 == 1; };
	puts("Odss Number from 1'000'000 to 1'001'000 displayed each 100th");

	for (int i : std::views::iota(1'000'000, 1'001'000) | std::views::filter(odd))
	{
		if (i % 100 == 1)
		{
			std::printf("%d ", i);
		}
	}

	puts("\n\nPrime Numbers from 1'000'000 to 1'001'000 displayed each 100th");
	constexpr auto isPrime = [](int i) constexpr
	{
		for (std::size_t j = 2; j * j <= i; ++j)
		{
			if (i % j == 0)
			{
				return false;
			}
		}
		return true;
	};

	for (auto i : std::views::iota(1'000'000, 1'001'000) | std::views::filter(isPrime))
	{
		std::printf("%d ", i);
	}


	puts("\n\n20 Prime Numbers starting from 1'000'000");

	for (auto i : std::views::iota(1'000'000, 1'001'000) | std::views::filter(isPrime) |std::views::take(20))
	{
		std::printf("%d ", i);
	}

}

inline void RangeAdaptors()
{
	std::printf("\n--RangeAdaptors--\n");

	std::vector<int>numbers{ 1,2,3,4,5,7,8,9 };

	auto filtered = numbers | std::views::filter([](auto number) { return number % 2 != 0; });

	auto transformed = std::views::transform([](auto elem) {return elem * 2; });

	auto results = transformed(filtered);

	for (const auto& number : results)
	{
		std::printf("%d ", number);
	}
	std::puts("");
	
}