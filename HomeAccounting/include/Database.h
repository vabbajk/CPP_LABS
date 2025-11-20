#pragma once
#include <fstream>
#include <string>
#include <memory>
#include "Transaction.h"


class TransactionList;

class Database {
private:
    std::string filename;
    std::fstream file;
    

    bool writeString(const std::string& str);
    bool writeDate(const Date& date);
    bool writeDouble(double value);
    bool writeSize(size_t value);
    
    std::string readString();
    Date readDate();
    double readDouble();
    size_t readSize();
    

    bool writeTransaction(const Transaction& transaction);
    std::shared_ptr<Transaction> readTransaction();
    

    bool isFileEmpty();
    void seekToEnd();
    void seekToBeginning();
    
public:
    explicit Database(const std::string& filename);
    ~Database() noexcept;
    

    bool saveTransactionList(const TransactionList& transactionList);
    TransactionList loadTransactionList();
    

    bool isOpen() const;
    bool openFile();
    void closeFile();
    bool createFile();
};