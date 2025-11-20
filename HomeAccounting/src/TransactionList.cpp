#include "../include/TransactionList.h"
#include "../include/TransactionQueries.h"

TransactionList::TransactionList() : dbFilename("") {

}

TransactionList::TransactionList(const std::string& dbFilename) : dbFilename(dbFilename) {
    loadFromDatabase();
}

double TransactionList::getCurrentMonthExpenses() const {
    return TransactionQueries::currentMonthExpenses(transactions);
}

double TransactionList::getCurrentMonthIncome() const {
    return TransactionQueries::currentMonthIncome(transactions);
}

double TransactionList::getCurrentMonthNetSavings() const {
    return TransactionQueries::currentMonthNetSavings(transactions);
}

double TransactionList::getTotalSavings() const {
    return TransactionQueries::totalSavings(transactions);
}