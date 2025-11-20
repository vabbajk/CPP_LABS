#pragma once
#include <list>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Transaction.h"
#include "Database.h"

class TransactionList {
private:
    std::list<std::shared_ptr<Transaction>> transactions;
    std::string dbFilename;

public:
    TransactionList();
    explicit TransactionList(const std::string& dbFilename);
    ~TransactionList() = default;


    void addTransaction(const std::shared_ptr<Transaction>& transaction) {
        transactions.push_back(transaction);
    }
    

    void addTransactionDirectly(const std::shared_ptr<Transaction>& transaction) {
        transactions.push_back(transaction);
    }
    

    bool updateTransaction(size_t id, const std::shared_ptr<Transaction>& newTransaction) {
        auto it = std::find_if(transactions.begin(), transactions.end(),
            [id](const std::shared_ptr<Transaction>& t) {
                return t->getID() == id;
            });
        
        if (it != transactions.end()) {
            *it = newTransaction;
            return true;
        }
        return false;
    }
    
    bool deleteTransaction(size_t id) {
        transactions.remove_if([id](const std::shared_ptr<Transaction>& t) {
            return t->getID() == id;
        });
        return true;
    }
    

    void loadFromDatabase() {
        Database db(dbFilename);
        TransactionList loaded = db.loadTransactionList();
        transactions = loaded.getAllTransactions();
    }
    
    void saveToDatabase() {
        Database db(dbFilename);
        db.saveTransactionList(*this);
    }
    

    std::list<std::shared_ptr<Transaction>> getAllTransactions() const {
        return transactions;
    }

    void removeByName(const std::string& name) {
        transactions.remove_if([name](const std::shared_ptr<Transaction>& t) {
            return t->getName() == name;
        });
    }

    std::list<std::shared_ptr<Transaction>> findByName(const std::string& name) const {
        std::list<std::shared_ptr<Transaction>> result;
        for (const auto& t : transactions) {
            if (t->getName() == name)
                result.push_back(t);
        }
        return result;
    }

    std::list<std::shared_ptr<Transaction>> findByCategory(const std::string& category) const {
        std::list<std::shared_ptr<Transaction>> result;
        for (const auto& t : transactions) {
            if (t->getCategory() == category)
                result.push_back(t);
        }
        return result;
    }

    std::list<std::shared_ptr<Transaction>> findByDate(const Date& date) const {
        std::list<std::shared_ptr<Transaction>> result;
        for (const auto& t : transactions) {
            if (t->getDate() == date)
                result.push_back(t);
        }
        return result;
    }

    void printAll() const {
        if (transactions.empty()) {
            std::cout << "������ ���������� ����.\n";
            return;
        }
        for (const auto& t : transactions)
            t->print();
    }

    double getTotalBalance() const {
        double total = 0.0;
        for (const auto& t : transactions) {
            if (t->getType()) total += t->getAmount();
            else total -= t->getAmount();
        }
        return total;
    }

    void sortByDate() {
        transactions.sort([](const std::shared_ptr<Transaction>& a, const std::shared_ptr<Transaction>& b) {
            return a->getDate() < b->getDate();
        });
    }

    void sortByAmount() {
        transactions.sort([](const std::shared_ptr<Transaction>& a, const std::shared_ptr<Transaction>& b) {
            return a->getAmount() < b->getAmount();
        });
    }

    void reverseOrder() {
        transactions.reverse();
    }

    void clear() {
        transactions.clear();
    }

    bool empty() const {
        return transactions.empty();
    }

    size_t size() const {
        return transactions.size();
    }


    double getCurrentMonthExpenses() const;
    double getCurrentMonthIncome() const;
    double getCurrentMonthNetSavings() const;
    

    double getTotalSavings() const;
};
