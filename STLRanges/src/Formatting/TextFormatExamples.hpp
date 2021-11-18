#pragma once
#include "RangesPCH.hpp"
//#include "Formatting/CustomFormatter.hpp"
//#include "Formatting/CustomFormatterV2.hpp"
//#include "Formatting/CustFormatStockIndexV3.hpp"
#include "Formatting/StockIndex.hpp"
#include "Formatting/CustomFormatVector.hpp"
#include "Formatting/CustomFormatLogger.hpp"
#include "Formatting/CompileTimeFormatSpecifier.hpp"

inline void oldwayFormatting()
{
	std::printf("\n--oldwayFormatting--\n");

	std::tuple values = { "Salim", 20123.13f, 2345.55, 1455.67f };

	fmt::print("{}\n", values);
	//old style formatting -8s; indicates a left aligned with a width of 8; 
	// %% is to print a percentage second one is to make sure it is not a format sign
	// printf is thread safe atomic and it prints all arguments at once as variadic therfore no interleavinf btween threads
	// but it is not type safe; not checks for types; 
	std::printf("%-8s, %8.2lf, %6.2lf, %4.2lf%%\n", std::get<0>(values), std::get<1>(values), std::get<2>(values), std::get<3>(values));

	fmt::print("{}", std::format("{:8}, {:>8.2f}, {:6.2f} {:.2f}% \n", 
							std::get<0>(values), std::get<1>(values), std::get<2>(values), std::get<3>(values)));

	std::string buffer;
	std::format_to(std::back_inserter(buffer), "Hello std::format {}", " c++20!");
	fmt::print("{}\n", buffer);
	buffer.clear();

	// format_to_n specifies the number of args , it does not take account if there is more args; e.g prints only Hello (5 chars)
	std::format_to_n(std::back_inserter(buffer), 5, "Hello std::format {}", " c++20!");
	fmt::print("{}\n", buffer);
}

inline void FormatwithLocales()
{
	std::printf("\n--FormatwithLocale--\n");
	using namespace std::string_literals;
	using namespace std::string_view_literals;


	// }} is an escape sequence; only } is printed
	fmt::print("{}", std::format("Having the  }} in a {}.\n", "string"));

	double pi = 3.14;
	int i = 1'024;

	auto us = "en_US.UTF-8"s;
	//de: is the language; German, en for English
	//_area = DE or US is the area
	//.code = is the code defines character encoding UTF-8 is Universal Character Set and Unicode standards; used on the internet as well
	// @modifier = is the modifier ; optional ; is allowed on some platforms to specify additional modifications, such as @euro for
	// using the euro symbol or @phone for sorting according to the telephone directory ;e.g.; de_DE.UTF-8@euro
	// language[_area[.code]][@modifier]
	auto locDE = std::locale("de_DE.UTF-8");
	auto locUS = std::locale(us);

	fmt::print("double format with locales:\n");
	fmt::print("{}", std::format(locDE, "\t\t in DE: {:L}\n", pi));
	fmt::print("{}", std::format(locUS, "\t\t in US: {:L}\n", pi));
	
	fmt::print("int-seperator format with locales:\n");
	fmt::print("{}", std::format(locDE, "\t\t in DE: {:L}\n", i));
	fmt::print("{}", std::format(locUS, "\t\t in US: {:L}\n", i));

}

inline void FormatFillAlign()
{
	std::printf("\n--FormatFillAlign--\n");

	int num = 2021;

	fmt::print("{}\n", std::format("{:6}", num));
	fmt::print("{}\n", std::format("{:6}", 'x'));
	fmt::print("{}\n", std::format("{:*<6}", 'x'));
	fmt::print("{}\n", std::format("{:*>6}", 'x'));
	fmt::print("{}\n", std::format("{:*^6}", 'x'));
	fmt::print("{}\n", std::format("{:6d}",num));
	
	// converts 'x' into numeric via ASCI Code; 'x'=120
	fmt::print("{}\n", std::format("{:6d}",'x'));  
	fmt::print("{}\n", std::format("{:6}", true));
}

inline void FormatSign()
{
	std::printf("\n--FormatSign--\n");

	fmt::print("{}\n", std::format("{0:}, {0:+}, {0:-}, {0: }", 0));
	fmt::print("{}\n", std::format("{0:}, {0:+}, {0:-}, {0: }", -0));
	fmt::print("{}\n", std::format("{0:}, {0:+}, {0:-}, {0: }", 1));
	fmt::print("{}\n", std::format("{0:}, {0:+}, {0:-}, {0: }", -1));
	
}

// Not compiling the custom format because of the MSVC bug in std::format_to referring to function Grow() in fmt_buffer_iterator!!
inline void CustomFormat_V1()
{
	std::printf("\n--CustomFormat_V1--\n");

	for (const auto& index : GetIndices())
	{
		//fmt::print("{}", std::format("{}\n", index));

		//std::cout << std::format("{}\n", index);
		std::cout << index.name() << " " << index.points() << "  "
			<< index.pointsDiff() << "  " << index.pointsPercent()
			<< '%' << '\n';
	}

	// Example from the book does not compile 
	//for (const auto& index : GetIndices()) {
	//	std::cout <<"custom format revised:"<< std::format("{}\n", index);
	//}

}

inline void CustomFormat_Vector()
{
	std::printf("\n-- CustomFormat_Vector--\n");

	std::vector<int>myInts{ 1,2,3,4,5,6,7,8,9,10 };
	
	// used for testing purposes!!
	//std::cout << std::format("{:}", myInts);
	//std::cout << fmt::format("{:}\n", myInts);
	fmt::print("{}", fmt::format("{:}\n", myInts));

	// example from Nico's c++20 book
	fmt::print("{}", std::format("{} is {}\n", 42, Color::red));

	std::vector<std::string> myStrings{ "Only", "for", "test", "purposes" };
	fmt::print("{}", fmt::format("{:}", myStrings));
}

 inline void WidthPrecisionSpecifier()
{
	std::printf("\n--WidthPrecisionSpecifier--\n");
	int i = 123456789;
	double d = 123.456789;
	fmt::print("---{}---\n", std::format("{}", i));
	fmt::print("---{}---\n", std::format("{:15}", i));
	fmt::print("---{}---\n", std::format("{:{}}", i, 15));
	
	fmt::print("---{}---\n", std::format("{}", d));
	fmt::print("---{}---\n", std::format("{:15}", d));
	fmt::print("---{}---\n", std::format("{:{}}", d, 15));

	// width=10, precision=50
	fmt::print("---{}---\n", std::format("{:10.50}", d));
	fmt::print("---{}---\n", std::format("{:{}.{}}", d, 10, 50));

	// width=10, precision=5
	fmt::print("---{}---\n", std::format("{:10.5}", d));
	fmt::print("---{}---\n", std::format("{:{}.{}}", d, 10, 5));

	std::string s = "Onyl a test";

	// precision = 500; no effect since it is bigger than the size of string
	fmt::print("---{}---\n", std::format("{:.500}", s));
	fmt::print("---{}---\n", std::format("{:.{}}", s, 500));

	// precision for string truncates the display if precision < length of string
	fmt::print("---{}---\n", std::format("{:.5}", s));
	fmt::print("---{}---\n", std::format("{:.{}}", s, 5));


}

 // Not compiling the custom format because of the MSVC bug in std::format_to referring to function Grow() in fmt_buffer_iterator!!
 inline void CustomFormat_V2()
 {
	 //std::printf("\n--CustomFormat_V1--\n");

	 //auto tempVec = GetIndices();

	 //// regular print format with no plus sign
	 //for (const auto& index : tempVec)
	 //{
		// fmt::print("{}\n", std::format("{}", index));
	 //}

	 //// custom format identifier 's' for a short form; only name and index printed
	 //for (const auto& index : tempVec)
	 //{
		// fmt::print("{}\n", std::format("{:s}", index));
	 //}

	 //// custom format identifier 'p' for a long form with a Plus sign
	 //for (const auto& index : tempVec)
	 //{
		// fmt::print("{}\n", std::format("{:p}", index));
	 //}
 }

 inline void DecHexOctTypeSpecifier()
 {
	 std::printf("\n-DecHexOctTypeSpecifier-\n");

	 int testnum = 2021;
	 fmt::print("default: {}\n", std::format("{:}", testnum));
	 fmt::print("decimal: {}\n", std::format("{:d}", testnum));
	 fmt::print("binary: {}\n", std::format("{:#b}", testnum));
	 fmt::print("hexadec: {}\n", std::format("{:#x}", testnum));
	 fmt::print("octal: {}\n", std::format("{:o}", testnum));

 }

 // Not compiling the custom format because of the MSVC bug in std::format_to referring to function Grow() in fmt_buffer_iterator!!
 inline void custformatStockIndex_V3()
 {
	// std::printf("\n--custformatStockIndex_V3--\n");

	// for (const auto& index : GetIndices())
	// {
	//	 fmt::print("{}\n", std::format("{:Ls}", index));
	// }

	// for (const auto& index : GetIndices())
	// {
	//	 fmt::print("{}\n", std::format("{:Lp}", index));
	// }

	 //for (const auto& index : GetIndices()) {
		// std::cout << std::format("{:Ls}\n", index);
	 //}

	 //for (const auto& index : GetIndices()) {
		// std::cout << std::format("{:Lp}\n", index);
	 //}
 }

 struct Share
 {
	 std::string name{};
	 double price{};
	 double priceDelta{};
 };

 inline void formatArgumentOrder()
 {
	 std::printf("\n--formatArgumentOrder--\n");

	 std::vector<Share> myShares= { {"KOC", 220.22, 50.5} };

	 auto locDE = std::locale("de_DE.UTF-8");

	 for (const auto& share : myShares)
	 {
		 fmt::print("{}\n", std::format(locDE, "{1:10} {2:>8.2f}{0:} {3:>+8.2f}{0:}", "$", share.name, share.price, share.priceDelta));
		
	 }
 }

 inline void customFormatBuffer()
 {
	 using namespace std::string_view_literals;
	 std::printf("\n--formatArgumentOrder--\n");
	
	 constexpr std::string_view fmt{ "{}, {}" };
	 
	 const auto size = std::formatted_size(fmt, "Hello"sv, "World"sv);

	 std::vector<char> buffer(size);

	 std::format_to(buffer.begin(), "{}, {}", "Hello", "World");

	 fmt::print("{}\n", std::string_view{ buffer.data(), size });

	 for (const auto& c : buffer)
	 {
		 fmt::print("{}", c);
	 }
	 puts(" ");

	 std::format_to(buffer.begin(), "{}, {}", "Hello", "World");

 }

 inline void FormatTofixedSizedBuffer()
 {
	 using namespace std::string_view_literals;
	 std::printf("\n-- FormatTofixedSizedBuffer--\n");

	 std::array<char, 11>buffer{};

	 std::format_to_n(buffer.data(), buffer.size(), "{} {}", "Hello", "World");

	 fmt::print("{}\n", std::string_view{ buffer.data(), buffer.size() });
 }

 inline void customLogger()
 {
	 std::printf("\n--customLogger--\n");

	 custLoggger(LogLevel::Warning, "{}, {}\n", "hello", "logger"); 
	 custLoggerV2(LogLevel::Error, "{}, {}\n", "hello", "logger");

	 int x{ 4 };
	 std::string share{ "Amazon" };
	 double price{ 3'117.45 };

	 custLoggerV2(LogLevel::Info, "Share price {0} is very high {1}", share, price);

	 std::array<char, 4>testbrace{ "{} " };
	 fmt::print("size: {1}, size of each elem: {2}, \nelems: {0} \n", testbrace, testbrace.size(), (sizeof testbrace[0]));

	 for (const auto& c : testbrace)
	 {
		 fmt::print("{}\n", c);
	 }

	 custLoggerV3(LogLevel::Info , "Share price:", share, " very high: ", price);
 }
