#pragma once

#include <list>
#include <memory>
#include <ctime>
#include <chrono>

#include "Transaction.h"
#include "AnalyticsUtils.h"

class TransactionQueries {
public:
    static double currentMonthExpenses(const std::list<std::shared_ptr<Transaction>>& transactions) {
        using std::chrono::system_clock;
        auto now = system_clock::now();
        std::time_t timeNow = system_clock::to_time_t(now);
        std::tm timeinfo{};

#ifdef _WIN32
        localtime_s(&timeinfo, &timeNow);
#else
        localtime_r(&timeNow, &timeinfo);
#endif

        int currentYear = timeinfo.tm_year + 1900;
        int currentMonth = timeinfo.tm_mon + 1;

        auto predicate = [currentYear, currentMonth](const std::shared_ptr<Transaction>& transaction) {
            if (transaction->getType() != 0) return false;
            const Date& transDate = transaction->getDate();
            return transDate.getYear() == currentYear &&
                   transDate.getMonth() == currentMonth;
        };

        return analytics::sum_if(
            transactions.begin(),
            transactions.end(),
            predicate,
            [](const std::shared_ptr<Transaction>& transaction) {
                return transaction->getAmount();
            }
        );
    }

    static double currentMonthIncome(const std::list<std::shared_ptr<Transaction>>& transactions) {
        using std::chrono::system_clock;
        auto now = system_clock::now();
        std::time_t timeNow = system_clock::to_time_t(now);
        std::tm timeinfo{};

#ifdef _WIN32
        localtime_s(&timeinfo, &timeNow);
#else
        localtime_r(&timeNow, &timeinfo);
#endif

        int currentYear = timeinfo.tm_year + 1900;
        int currentMonth = timeinfo.tm_mon + 1;
        int currentDay = timeinfo.tm_mday;

        Date monthStart(1, currentMonth, currentYear);
        Date today(currentDay, currentMonth, currentYear);

        auto predicate = [currentYear, currentMonth, monthStart, today](const std::shared_ptr<Transaction>& transaction) {
            if (transaction->getType() != 1) return false;
            Date transDate = transaction->getDate();
            return transDate.getYear() == currentYear &&
                   transDate.getMonth() == currentMonth &&
                   transDate >= monthStart &&
                   transDate <= today;
        };

        return analytics::sum_if(
            transactions.begin(),
            transactions.end(),
            predicate,
            [](const std::shared_ptr<Transaction>& transaction) {
                return transaction->getAmount();
            }
        );
    }

    static double currentMonthNetSavings(const std::list<std::shared_ptr<Transaction>>& transactions) {
        analytics::ValueAccumulator<double> acc;
        acc.add(currentMonthIncome(transactions));
        acc.add(-currentMonthExpenses(transactions));
        return acc.total();
    }

    static double totalSavings(const std::list<std::shared_ptr<Transaction>>& transactions) {
        double totalIncome = 0.0;
        double totalExpenses = 0.0;

        for (const auto& transaction : transactions) {
            if (transaction->getType() == 1) {
                totalIncome += transaction->getAmount();
            } else {
                totalExpenses += transaction->getAmount();
            }
        }

        return totalIncome - totalExpenses;
    }
};
