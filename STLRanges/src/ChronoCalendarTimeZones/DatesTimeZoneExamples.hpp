#pragma once
#include "RangesPCH.hpp"

inline void timeofDay()
{
	std::printf("\n--- timeofDay---\n");

	using namespace std::chrono_literals;

	// below construct gives a total duration which is the time from midnight; e.g: 10h will 10::00 am in the morning
	auto timeofDay = std::chrono::hh_mm_ss(10.5h + 98min + 2020s+ 0.5s);

	fmt::print("timeofDay: {}\n", timeofDay);
	fmt::print("timeofDay.hours(): {}\n", timeofDay.hours());
	fmt::print("timeofDay.minutes(): {}\n", timeofDay.minutes());
	fmt::print("timeofDay.seconds(): {}\n", timeofDay.seconds());
	fmt::print("timeofDay.subseconds(): {}\n", timeofDay.subseconds());
	fmt::print("timeofDay.to_duration(): {}\n", timeofDay.to_duration());

	fmt::print("std::chrono::hh_mm_ss(45700.5s) {}\n", std::chrono::hh_mm_ss(45700.5s));

	fmt::print("std::chrono::is_am(5h): {}\n", std::chrono::is_am(5h));
	fmt::print("std::chrono::is_pm(15h): {}\n", std::chrono::is_pm(15h));

	fmt::print("std::chrono::make12(5h): {}\n", std::chrono::make12(5h));
	fmt::print("std::chrono::make12(0h): {}\n", std::chrono::make12(0h));
	fmt::print("std::chrono::make24(5h, true): {}\n", std::chrono::make24(5h, true));
	fmt::print("std::chrono::make24(5h, true): {}, pm?: {}\n", std::chrono::make24(5h, true), std::chrono::is_pm(std::chrono::make24(5h, true)));
}

inline void calendarDates()
{
	std::printf("\n--- calendarDates---\n");
	using namespace std::chrono_literals;

	constexpr auto yearMonthDay = std::chrono::year_month_day(std::chrono::year{ 1920 }, std::chrono::month{ 12 }, std::chrono::day{ 15 });
	fmt::print("std::chrono::year_month_day(1920, 12, 15): {}\n", yearMonthDay);

	constexpr auto yearMonthDay2 = std::chrono::year_month_day(std::chrono::year{ 1920 }/std::chrono::month{ 12 }/std::chrono::day{ 15 });
	fmt::print("std::chrono::year_month_day(1920/12/15): {}\n", yearMonthDay2);

	constexpr auto yearMonthDay3 = std::chrono::year_month_day( 1920y/12/15);
	fmt::print("std::chrono::year_month_day(1920/12/15): {}\n", yearMonthDay3);

	constexpr auto yearMonthDay4 = std::chrono::year{1920} / 12 / 15;
	fmt::print("std::chrono::year(1920)/12/15: {}\n", yearMonthDay4);

	constexpr auto yearMonthDay5 = 1920y/12/15;
	fmt::print("1920y/12/15: {}\n", yearMonthDay5);

	constexpr auto yearMonthDay6 = 1920y / std::chrono::December / 15;
	fmt::print("1920y/December/15: {}\n", yearMonthDay6);

	constexpr auto yearMonthDayLast = std::chrono::year{ 2010 } / std::chrono::March / std::chrono::last;
	fmt::print("2010/March/LastDay: {}\n", yearMonthDayLast);

	auto yearMonthWeekday{ std::chrono::year{2021} / std::chrono::January / std::chrono::Wednesday[1]};
	fmt::print("2021/January/Wednesday[1]: {}\n", std::chrono::year_month_day(yearMonthWeekday));

	constexpr auto ym{ std::chrono::year(2021) / std::chrono::January };
	constexpr auto wdi{ std::chrono::Wednesday[1] };
	auto ymwdi{ ym / wdi };
	auto index = ymwdi.index() + 1;
	auto weekday = ymwdi.weekday() + std::chrono::days(1);
	ymwdi = { ymwdi.year() / ymwdi.month() /weekday[index] };
	fmt::print("2021/January/2ndThursday (14th): {}\n", std::chrono::year_month_day(ymwdi));

	constexpr auto yearMonthWeekdayLast = std::chrono::year(2010) / std::chrono::March / std::chrono::Monday[std::chrono::last];
	fmt::print("yearMonthWeekdayLast: {0}: {1}\n", yearMonthWeekdayLast, std::chrono::year_month_day(yearMonthDayLast));

	// better to use namespace std::chrono!!
	fmt::print("yearMonthWeekdayLast: {0}\n", std::chrono::year_month_weekday_last(2010y, std::chrono::month(std::chrono::March), std::chrono::weekday_last(std::chrono::Monday)));

}

inline void calendarDates2()
{
	std::printf("\n--- calendarDates2---\n");
	using namespace std::chrono_literals;

	constexpr auto day1 = std::chrono::day(19);	
	constexpr auto day2{ 12d };
	fmt::print("day1: {0}, day2: {1}\n", day1, day2);

	constexpr auto year1 = std::chrono::year{ 2021 };
	fmt::print("year1: {0}\n", year1);
	
	constexpr auto month1 = std::chrono::March;
	constexpr auto month2 = std::chrono::month{ 3 };
	fmt::print("month1: {0}; month2: {1}\n", month1, month2);

	constexpr auto weekday1 = std::chrono::weekday{ 2 };
	constexpr auto weekday2 = std::chrono::Tuesday;
	constexpr auto weekday_indexed3 = std::chrono::Tuesday[2];
	fmt::print("weekday1: {0}; weekday2: {1}, weekday-indexed3: {2}\n", weekday1, weekday2, weekday_indexed3);
	fmt::print("weekday_indexed3.weekday(): {0}, weekday_indexed3.index(): {1}\n", weekday_indexed3.weekday(), weekday_indexed3.index());

}

inline void systemDaysExample()
{
	std::printf("\n--systemDays--\n");
	using namespace std::chrono_literals;
	using namespace std::chrono;

	constexpr auto yearMonthDayLast{ 2021y / March / std::chrono::last };
	fmt::print("sys_days: {0}, yearMonthDayLast: {1}\n", std::chrono::sys_days(yearMonthDayLast), yearMonthDayLast);

	constexpr auto leapDate{ 2012y / February / std::chrono::last };
	fmt::print("sys_days: {0}, yearMonthDayLast: {1}\n", std::chrono::sys_days(leapDate), leapDate);

	fmt::print("is {} a leap year : {}\n", std::chrono::sys_days(leapDate), leapDate.year().is_leap());

	std::chrono::sys_days start = 2021y / 1 / 5;
	fmt::print("start: {}\n", start);
	
	// NOTE: Dont use moths and years with time_points and system clock since months / years use
	// an average duration of a month therefore the type end and start is not exactly the same end has some fractions
	// It is better to use weeks or days
	auto end = start + months(1);  // end= 2021-02-04 10:29:06
	fmt::print("end: {}\n", end);

	auto end2 = start + weeks(4);
	fmt::print("end2: {}\n", end2); // end2= 2021-02-02
}

inline void checkValidDates()
{
	std::printf("\n--checkValidDates--\n");
	using namespace std::chrono_literals;
	using namespace std::chrono;

	std::chrono::day day31{ 32 };
	fmt::print("day31==32 ok ?: {}, {}\n", day31 , day31.ok());

	std::chrono::month monthDec{ 12 };
	std::chrono::month checkMonth = monthDec + std::chrono::months(1);
	fmt::print("monthDec ok ?: {}, monthDec +1 ok?: {}\n", monthDec.ok(), checkMonth);

	std::chrono::month month13{ 13 };
	fmt::print("month13 ok ? : {}, {}\n", month13, month13.ok());

	std::chrono::year year2010{ 2010};
	std::chrono::year yearNeg32768 { -32768 };
	// The class year represents a year in the proleptic Gregorian calendar. Its range is [-32767, 32767].
	fmt::print("year 2010 ok ?: {}, year -32768 ok ? : {}\n", year2010, yearNeg32768.ok());

	constexpr year leapYear2010{ 2010 };
	constexpr year leapYear2012{ 2012 };
	constexpr year leapYear2013{ 2013};
	constexpr year leapYear2023{ 2023};

	fmt::print("leapYear2010 ?: {}, leapYear2012 ?: {}, leapYear2013 ?: {}, leapYear2023 ?: {}\n", leapYear2010.is_leap(), leapYear2012.is_leap(), leapYear2013.is_leap(), leapYear2023.is_leap());
}

inline void querryCalendarDates()
{
	std::printf("\n--querryCalendarDates--\n");

	using namespace std::chrono_literals;
	using namespace std::chrono;

	auto birthDay = 1972y / std::chrono::month(03)/27d;

	if (not birthDay.ok())
	{
		fmt::print("not a valid date: {}\n", birthDay);
		std::exit(EXIT_FAILURE);
	}

	fmt::print("birthday: {}\n", birthDay);

	auto birthdayWeekday = std::chrono::year_month_weekday{ birthDay };

	fmt::print("weekday of birthday({0}) : {1}\n", birthDay, birthdayWeekday.weekday());

	// std::chrono::floor<days> rounds the given system_clock::now to days;; it is a rounding function
	auto currentDate= std::chrono::year_month_day(std::chrono::floor<days>(std::chrono::system_clock::now()));
	auto currentYear = currentDate.year();

	auto age = std::chrono::duration_cast<years>(currentDate.year()- birthDay.year()).count();

	//this is the way in the book; less overhead
	auto age2 = (int)currentDate.year() - (int)birthDay.year();
	fmt::print("age: {}, age2: {}\n", age, age2);

	puts("Next 10 birthdays and weekdays: ");
	for (int i = 1, newYear = (int)currentYear; i <= 10; ++i)
	{
		fmt::print("Age: {}\n", ++age);
		auto NewBirthDay = std::chrono::year{ ++newYear } / birthDay.month() / birthDay.day();
		fmt::print("\tNew Birthday: {}\n", NewBirthDay);
		fmt::print("\tweekday: {}\n", std::chrono::year_month_weekday{ NewBirthDay }.weekday());

	}
}

inline void calculateOrdinalDates()
{
	std::printf("\n--calculateOrdinalDates--\n");

	using namespace std::chrono_literals;
	using namespace std::chrono;

	const auto time = std::chrono::system_clock::now();
	const auto daypoint = std::chrono::floor<days>(time);
	const auto yearMonthday = std::chrono::year_month_day{ daypoint };

	// calculate the year and day of the year

	const auto currentYear = yearMonthday.year();
	const auto year_Day = daypoint - std::chrono::sys_days{ currentYear / January / 0d };

	fmt::print("currentYear: {0}, day_count: {1}\n", currentYear, year_Day.count());

	// inverting the calculation to check
	assert(yearMonthday == std::chrono::year_month_day{ std::chrono::sys_days{currentYear / January / 0} + year_Day});

}






