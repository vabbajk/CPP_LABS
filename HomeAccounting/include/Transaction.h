#pragma once
#include <string>
#include <utility>
#include <iostream>
#include "HelpfulClasses.h"

class Transaction {
    private:
        static size_t nextID;
        size_t id;
        std::string name;
        std::string category;
        Date date;
        double amount;

    public:
        Transaction(const std::string& name, const std::string& category, Date date, double amount);
        Transaction(size_t existingID, const std::string& name, const std::string& category, Date date, double amount);
        virtual ~Transaction() = default;

        virtual void print() = 0;
        virtual int getType() = 0;

        double getAmount() const;
        size_t getID() const;
        std::string getName() const;
        std::string getCategory() const;
        Date getDate() const;
        
        static void updateNextID(size_t maxID);
};

class IncomeTransaction : public Transaction {
    private:
        std::string incomeSource;
    public:
        IncomeTransaction(const std::string& name, const std::string& category, Date date, double amount, const std::string& incomeSource);
        IncomeTransaction(size_t existingID, const std::string& name, const std::string& category, Date date, double amount, const std::string& incomeSource);
        ~IncomeTransaction() override = default;

        void print() override;
        int getType() override;
        

        std::string getIncomeSource() const;

        friend std::ostream& operator<<(std::ostream& os, const IncomeTransaction& it) {
			os << ' '
			   << it.getName() << ' '
			   << it.getCategory() << ' '
			   << it.getDate() << ' '
			   << it.getAmount() << ' '
			   << it.incomeSource << std::endl;
            return os;
        }
};

class Expense : public Transaction {
private:
    std::string where;
public:
    Expense(const std::string& name, const std::string& category, Date date, double amount, const std::string& where);
    Expense(size_t existingID, const std::string& name, const std::string& category, Date date, double amount, const std::string& where);
    ~Expense() override = default;

    void print() override;
    int getType() override;
    

    std::string getWhere() const;

    friend std::ostream& operator<<(std::ostream& os, const Expense& it) {
		os << ' '
		   << it.getName() << ' '
		   << it.getCategory() << ' '
		   << it.getDate() << ' '
		   << it.getAmount() << ' '
		   << it.where << std::endl;
        return os;
    }
};