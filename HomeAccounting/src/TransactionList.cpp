#include "../include/TransactionList.h"
#include "../include/AnalyticsUtils.h"
#include <ctime>

TransactionList::TransactionList() : dbFilename("") {

}

TransactionList::TransactionList(const std::string& dbFilename) : dbFilename(dbFilename) {
    loadFromDatabase();
}

double TransactionList::getCurrentMonthExpenses() const {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    int currentYear = timeinfo->tm_year + 1900;
    int currentMonth = timeinfo->tm_mon + 1;
    int currentDay = timeinfo->tm_mday;

    Date monthStart(1, currentMonth, currentYear);
    Date today(currentDay, currentMonth, currentYear);

    auto predicate = [&](const std::shared_ptr<Transaction>& transaction) {
        if (transaction->getType() != 0) return false;
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

double TransactionList::getCurrentMonthIncome() const {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    int currentYear = timeinfo->tm_year + 1900;
    int currentMonth = timeinfo->tm_mon + 1;
    int currentDay = timeinfo->tm_mday;

    Date monthStart(1, currentMonth, currentYear);
    Date today(currentDay, currentMonth, currentYear);

    auto predicate = [&](const std::shared_ptr<Transaction>& transaction) {
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

double TransactionList::getCurrentMonthNetSavings() const {
    analytics::ValueAccumulator<double> accumulator;
    accumulator.add(getCurrentMonthIncome());
    accumulator.add(-getCurrentMonthExpenses());
    return accumulator.total();
}

double TransactionList::getTotalSavings() const {
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