#include "../include/Transaction.h"


size_t Transaction::nextID = 1;

Transaction::Transaction(std::string name, std::string category, Date date, double amount) :
    id(nextID++), name(std::move(name)), category(std::move(category)), date(date), amount(amount){

    if (id == 0) {
        throw std::runtime_error("Ошибка инициализации ID транзакции");
    }
}


Transaction::Transaction(size_t existingID, std::string name, std::string category, Date date, double amount) :
    id(existingID), name(std::move(name)), category(std::move(category)), date(date), amount(amount){

    if (id == 0) {
        throw std::runtime_error("Ошибка инициализации ID транзакции");
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




IncomeTransaction::IncomeTransaction(std::string name, std::string category, Date date, double amount, std::string incomeSource):
    Transaction(name, category, date, amount), incomeSource(incomeSource) {}

IncomeTransaction::IncomeTransaction(size_t existingID, std::string name, std::string category, Date date, double amount, std::string incomeSource):
    Transaction(existingID, name, category, date, amount), incomeSource(incomeSource) {}

void IncomeTransaction::print(){}

int IncomeTransaction::getType(){
    return 1;
}

std::string IncomeTransaction::getIncomeSource() const {
    return incomeSource;
}


Expense::Expense(std::string name, std::string category, Date date, double amount, std::string where):
    Transaction(name, category, date, amount), where(where) {}

Expense::Expense(size_t existingID, std::string name, std::string category, Date date, double amount, std::string where):
    Transaction(existingID, name, category, date, amount), where(where) {}

void Expense::print() {}

int Expense::getType(){
    return 0;
}

std::string Expense::getWhere() const {
    return where;
}

