#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <format>
#include <compare>

class Date {
private:
    int day;
    int month;
    int year;

public:
    Date();
    Date(int day, int month, int year);
    ~Date() = default;

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    std::string getDate() const;

    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        return os << std::format("{:02}.{:02}.{}", date.day, date.month, date.year);
    }

    friend auto operator<=>(const Date& lhs, const Date& rhs) = default;
};

