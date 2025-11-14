#include "../include/Database.h"
#include "../include/TransactionList.h"
#include "../include/Transaction.h"
#include <iostream>
#include <cstring>
#include <filesystem>

Database::Database(const std::string& filename) : filename(filename) {
    std::cerr << "[DEBUG Database] Constructor called with filename: " << filename << std::endl;
    if (!openFile()) {
        std::cerr << "[ERROR Database] Не удалось открыть файл базы данных: " << filename << std::endl;
    } else {
        std::cerr << "[DEBUG Database] File opened successfully: " << filename << std::endl;
    }
}

Database::~Database() {
    std::cerr << "[DEBUG Database] Destructor called for file: " << filename << std::endl;
    closeFile();
}

bool Database::openFile() {
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {

        return createFile();
    }
    return true;
}

bool Database::createFile() {
    file.open(filename, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.close();
        return openFile();
    }
    return false;
}

void Database::closeFile() {
    if (file.is_open()) {
        file.close();
    }
}

bool Database::isOpen() const {
    return file.is_open();
}


bool Database::writeString(const std::string& str) {
    size_t length = str.length();
    if (!writeSize(length)) return false;
    if (!file.write(str.c_str(), static_cast<std::streamsize>(length))) return false;
    return file.good();
}

bool Database::writeDate(const Date& date) {
    int day = date.getDay();
    int month = date.getMonth();
    int year = date.getYear();

    if (!file.write(reinterpret_cast<const char*>(&day), sizeof(int))) return false;
    if (!file.write(reinterpret_cast<const char*>(&month), sizeof(int))) return false;
    if (!file.write(reinterpret_cast<const char*>(&year), sizeof(int))) return false;
    return file.good();
}

bool Database::writeDouble(double value) {
    if (!file.write(reinterpret_cast<const char*>(&value), sizeof(double))) return false;
    return file.good();
}

bool Database::writeSize(size_t value) {
    if (!file.write(reinterpret_cast<const char*>(&value), sizeof(size_t))) return false;
    return file.good();
}

std::string Database::readString() {
    size_t length = readSize();
    std::string str(length, '\0');
    file.read(&str[0], length);
    return str;
}

Date Database::readDate() {
    int day, month, year;
    file.read(reinterpret_cast<char*>(&day), sizeof(int));
    file.read(reinterpret_cast<char*>(&month), sizeof(int));
    file.read(reinterpret_cast<char*>(&year), sizeof(int));
    return Date(day, month, year);
}

double Database::readDouble() {
    double value;
    file.read(reinterpret_cast<char*>(&value), sizeof(double));
    return value;
}

size_t Database::readSize() {
    size_t value;
    file.read(reinterpret_cast<char*>(&value), sizeof(size_t));
    return value;
}

bool Database::writeTransaction(const Transaction& transaction) {
    if (!file.is_open() || !file.good()) {
        return false;
    }

    int type = -1;
    const IncomeTransaction* income = dynamic_cast<const IncomeTransaction*>(&transaction);
    const Expense* expense = dynamic_cast<const Expense*>(&transaction);

    if (income) {
        type = 0;
    } else if (expense) {
        type = 1;
    } else {
        return false;
    }

    file.write(reinterpret_cast<const char*>(&type), sizeof(type));
    if (!file.good()) return false;

    if (!writeSize(transaction.getID())) return false;
    if (!writeString(transaction.getName())) return false;
    if (!writeString(transaction.getCategory())) return false;
    if (!writeDate(transaction.getDate())) return false;
    if (!writeDouble(transaction.getAmount())) return false;

    if (income) {
        if (!writeString(income->getIncomeSource())) return false;
    } else if (expense) {
        if (!writeString(expense->getWhere())) return false;
    }

    return true;
}

std::shared_ptr<Transaction> Database::readTransaction() {

    int type;
    file.read(reinterpret_cast<char*>(&type), sizeof(int));
    if (!file.good()) {
        throw std::runtime_error("Failed to read transaction type");
    }
    

    size_t id = readSize();
    if (!file.good()) {
        throw std::runtime_error("Failed to read transaction ID");
    }
    

    std::string name = readString();
    std::string category = readString();
    Date date = readDate();
    double amount = readDouble();
    
    if (!file.good()) {
        throw std::runtime_error("Failed to read transaction basic data");
    }

    if (type == 0) {
        std::string incomeSource = readString();
        if (!file.good()) {
            throw std::runtime_error("Failed to read income source");
        }
        return std::static_pointer_cast<Transaction>(std::make_shared<IncomeTransaction>(id, name, category, date, amount, incomeSource));
    } else {
        std::string where = readString();
        if (!file.good()) {
            throw std::runtime_error("Failed to read expense location");
        }
        return std::static_pointer_cast<Transaction>(std::make_shared<Expense>(id, name, category, date, amount, where));
    }
}

bool Database::isFileEmpty() {
    file.seekg(0, std::ios::end);
    return file.tellg() == 0;
}

void Database::seekToEnd() {
    file.seekp(0, std::ios::end);
}

void Database::seekToBeginning() {
    file.seekg(0, std::ios::beg);
}

bool Database::saveTransactionList(const TransactionList& transactionList) {
    std::cerr << "[DEBUG] Saving to file: " << filename << std::endl;
    
    file.close();
    file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return false;
    }

    auto transactions = transactionList.getAllTransactions();
    std::cerr << "Saving " << transactions.size() << " transactions..." << std::endl;

    for (const auto& transaction : transactions) {
        if (!writeTransaction(*transaction)) {
            std::cerr << "Write failed!" << std::endl;
            system("pause");
            file.close();
            return false;
        }
    }

    file.flush();
    file.close();

    std::error_code ec;
    auto size = std::filesystem::file_size(filename, ec);
    if (ec) {
        std::cerr << "Cannot get file size: " << ec.message() << std::endl;
    } else {
        std::cerr << "File size after save: " << size << " bytes" << std::endl;
    }
    

    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[WARNING] Cannot reopen file after save: " << filename << std::endl;
    } else {
        std::cerr << "[DEBUG] File successfully reopened after save" << std::endl;
    }
    
    return true;
}

TransactionList Database::loadTransactionList() {
    TransactionList transactionList;
    
    std::cerr << "[DEBUG] Loading from file: " << filename << std::endl;
    

    if (!isOpen()) {
        std::cerr << "[DEBUG] File is closed, attempting to open..." << std::endl;
        if (!openFile()) {
            std::cerr << "[ERROR] Cannot open file for reading: " << filename << std::endl;
            return transactionList;
        }
    }
    
    if (isFileEmpty()) {
        std::cerr << "[DEBUG] File is empty" << std::endl;
        return transactionList;
    }
    
    seekToBeginning();
    
    int count = 0;
    size_t maxID = 0;
    
    while (file.good() && !file.eof()) {

        std::streampos pos = file.tellg();
        int test_type;
        file.read(reinterpret_cast<char*>(&test_type), sizeof(int));
        
        if (!file.good() || file.eof()) {

            break;
        }
        

        file.seekg(pos);
        
        try {
            auto transaction = readTransaction();
            transactionList.addTransactionDirectly(transaction);
            

            if (transaction->getID() > maxID) {
                maxID = transaction->getID();
            }
            
            count++;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка при чтении транзакции #" << (count + 1) << ": " << e.what() << std::endl;
            break;
        }
    }
    

    if (maxID > 0) {
        Transaction::updateNextID(maxID);
        std::cerr << "[DEBUG] Updated nextID to " << (maxID + 1) << std::endl;
    }
    
    std::cerr << "[DEBUG] Loaded " << count << " transactions" << std::endl;
    
    return transactionList;
}