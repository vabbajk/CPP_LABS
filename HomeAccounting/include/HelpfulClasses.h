#pragma once
#include <string>
#include <iostream>
#pragma once
#include <string>
#include <iostream>
#include <iomanip>

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
        os << std::setw(2) << std::setfill('0') << date.day << "."
           << std::setw(2) << std::setfill('0') << date.month << "."
           << date.year;
        return os;
    }

    friend bool operator==(const Date& lhs, const Date& rhs) {
        return lhs.day == rhs.day && lhs.month == rhs.month && lhs.year == rhs.year;
    }

    friend bool operator!=(const Date& lhs, const Date& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const Date& lhs, const Date& rhs) {
        if (lhs.year != rhs.year) return lhs.year < rhs.year;
        if (lhs.month != rhs.month) return lhs.month < rhs.month;
        return lhs.day < rhs.day;
    }

    friend bool operator>(const Date& lhs, const Date& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const Date& lhs, const Date& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>=(const Date& lhs, const Date& rhs) {
        return !(lhs < rhs);
    }
};

