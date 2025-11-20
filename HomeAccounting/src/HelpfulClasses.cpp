#include "../include/HelpfulClasses.h"

#include <chrono>
#include <ctime>
#include <format>

Date::Date(){
    using std::chrono::system_clock;
    auto now = system_clock::now();
    std::time_t timeNow = system_clock::to_time_t(now);
    std::tm local_tm{};

#ifdef _WIN32
    localtime_s(&local_tm, &timeNow);
#else
    localtime_r(&timeNow, &local_tm);
#endif

    day = local_tm.tm_mday;
    month = local_tm.tm_mon + 1;
    year = local_tm.tm_year + 1900;
}

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {}

int Date::getDay() const {
    return day;
}

int Date::getMonth() const {
    return month;
}

int Date::getYear() const {
    return year;
}

std::string Date::getDate() const{
	return std::format("{:02}.{:02}.{}", day, month, year);
}
