#pragma once
#include "RangesPCH.hpp"

inline void localTimeTest1()
{
	std::printf("\n--- localTime---\n");

	using namespace std::chrono_literals;
	using namespace std::chrono;

	fmt::print("ISA TimeZone tzdb version: {}\n", std::chrono::get_tzdb().version);

	auto utcTime = std::chrono::system_clock::now();
	fmt::print("UTC Time : {}\n", utcTime);

	auto localTime = std::chrono::zoned_time(std::chrono::current_zone(), utcTime);
	fmt::print("localTime: {0}\n", localTime);
	fmt::print("localTime(truncated to secs): {}\n", std::chrono::floor<std::chrono::seconds>(localTime.get_local_time()));

	auto offset = localTime.get_info().offset;
	fmt::print("UTC offset : {}\n", std::chrono::duration_cast<hours>(offset));

	auto timezoneHere = std::chrono::current_zone();
	auto timezoneUTC = std::chrono::locate_zone("Europe/Istanbul");

	fmt::print("timezoneHere (current_zone): {0} \nUTC; locate_zone: {1}\n", timezoneHere->name(), timezoneUTC->name());
}

template<typename ZonedTime>
auto getMinutes(const ZonedTime& zonedTime)
{
	return std::chrono::floor<std::chrono::minutes>(zonedTime.get_local_time());
}

void printStartEndTimes(const std::chrono::local_days& localDay, const std::chrono::hours& h, 
						const std::chrono::hours& durationClass, const std::initializer_list<std::string>& timeZones)
{
	std::chrono::zoned_time startDate{ std::chrono::current_zone(), localDay + h };
	std::chrono::zoned_time endDate{ std::chrono::current_zone(), localDay + h + durationClass };
	fmt::print("\nLocal Time({0}: [{1}, {2}]\n", std::chrono::current_zone()->name(), getMinutes(startDate), getMinutes(endDate));

	auto longestStringSize = std::max(timeZones, [](std::string_view elem1, std::string_view elem2) { return elem1.size() < elem2.size(); }).size();

	for (auto timeZone : timeZones)
	{
		fmt::print("timezone:{0} [{1}, {2}]\n", timeZone, getMinutes(std::chrono::zoned_time{ timeZone, startDate }), getMinutes(std::chrono::zoned_time{timeZone, endDate}));
	}
}

inline void variousTimeZones()
{
	std::printf("\n--variousTimeZones_ClassTimes--\n");

	using namespace std::string_literals;
	using namespace std::string_view_literals;
	using namespace std::chrono_literals;
	using namespace std::chrono;

	constexpr auto classDay = 2021y / 2 / 1 ;
	constexpr auto durationClass = 4h;

	auto timeZones = { "America/Los_Angeles"s,"America/Denver"s, "America/New_York"s,
					"Europe/London"s, "Europe/Istanbul"s, "Europe/Minsk"s, "Europe/Moscow"s,
					"Asia/Kolkata"s, "Asia/Novosibirsk"s, "Asia/Singapore"s,
					"Australia/Perth"s, "Australia/Sydney"s };

	for (auto startTime : { 7h, 13h, 17h })
	{
		printStartEndTimes(std::chrono::local_days{ classDay }, startTime, durationClass, timeZones);
	}
}

