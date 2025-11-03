#include "../include/HelpfulClasses.h"

#include <chrono>
#include <time.h>
#include <sstream>
#include <iomanip>

Date::Date(){
    time_t now = time(nullptr);
    tm* local = localtime(&now);

    day = local->tm_mday;
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;
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

