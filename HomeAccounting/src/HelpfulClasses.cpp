#include "../include/HelpfulClasses.h"

#include <chrono>
#include <time.h>
#include <sstream>
#include <iomanip>

Date::Date(){
    std::time_t now = std::time(nullptr);
    std::tm local_tm{};

    day = local_tm.tm_mday;
    month = local_tm.tm_mon + 1;
    year = local_tm.tm_year + 1900;
}

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {}

Date::~Date(){}

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
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << "."
        << std::setw(2) << std::setfill('0') << month << "."
        << year;
    return oss.str();
}

