#include "../include/Transaction.h"
#include <stdexcept>

namespace {
class TransactionIdException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
}

size_t Transaction::nextID = 1;

Transaction::Transaction(const std::string& name, const std::string& category, Date date, double amount) :
    id(nextID++), name(name), category(category), date(date), amount(amount){

    if (id == 0) {
        throw TransactionIdException("Ошибка инициализации ID транзакции");
    }
}


Transaction::Transaction(size_t existingID, const std::string& name, const std::string& category, Date date, double amount) :
    id(existingID), name(name), category(category), date(date), amount(amount){

    if (id == 0) {
        throw TransactionIdException("Ошибка инициализации ID транзакции");
    }
}

void Transaction::updateNextID(size_t maxID) {
    if (maxID >= nextID) {
        nextID = maxID + 1;
    }
}


double Transaction::getAmount() const {
    return amount;
}

size_t Transaction::getID() const {
    return id;
}

std::string Transaction::getName() const {
    return name;
}

std::string Transaction::getCategory() const {
    return category;
}

Date Transaction::getDate() const {
    return date;
}




IncomeTransaction::IncomeTransaction(const std::string& name, const std::string& category, Date date, double amount, const std::string& incomeSource):
    Transaction(name, category, date, amount), incomeSource(incomeSource) {}

IncomeTransaction::IncomeTransaction(size_t existingID, const std::string& name, const std::string& category, Date date, double amount, const std::string& incomeSource):
    Transaction(existingID, name, category, date, amount), incomeSource(incomeSource) {}

void IncomeTransaction::print(){
    std::cout << *this;
}

int IncomeTransaction::getType(){
    return 1;
}

std::string IncomeTransaction::getIncomeSource() const {
    return incomeSource;
}


Expense::Expense(const std::string& name, const std::string& category, Date date, double amount, const std::string& where):
    Transaction(name, category, date, amount), where(where) {}

Expense::Expense(size_t existingID, const std::string& name, const std::string& category, Date date, double amount, const std::string& where):
    Transaction(existingID, name, category, date, amount), where(where) {}

void Expense::print() {
    std::cout << *this;
}

int Expense::getType(){
    return 0;
}

std::string Expense::getWhere() const {
    return where;
}

