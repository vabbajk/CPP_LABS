#include "../include/Database.h"
#include "../include/TransactionList.h"
#include "../include/Transaction.h"
#include <iostream>
#include <cstring>
#include <filesystem>
#include <cstddef>
#include <stdexcept>

namespace {
class DatabaseReadException : public std::runtime_error {
public:
    explicit DatabaseReadException(const std::string& message)
        : std::runtime_error(message) {}
};
}

Database::Database(const std::string& filename) : filename(filename) {
    std::cerr << "[DEBUG Database] Constructor called with filename: " << filename << std::endl;
    if (!openFile()) {
        std::cerr << "[ERROR Database] Не удалось открыть файл базы данных: " << filename << std::endl;
    } else {
        std::cerr << "[DEBUG Database] File opened successfully: " << filename << std::endl;
    }
}

Database::~Database() noexcept {
    try {
        std::cerr << "[DEBUG Database] Destructor called for file: " << filename << std::endl;
        closeFile();
    } catch (const std::exception& e) {
        std::cerr << "[ERROR Database] Exception in destructor: " << e.what() << std::endl;
    }
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
    auto length = str.length();
    if (!writeSize(length)) return false;
    if (!file.write(str.c_str(), static_cast<std::streamsize>(length))) return false;
    return file.good();
}

bool Database::writeDate(const Date& date) {
    int day = date.getDay();
    int month = date.getMonth();
    int year = date.getYear();

    const auto* dayBytes = reinterpret_cast<const std::byte*>(&day);
    if (!file.write(reinterpret_cast<const char*>(dayBytes), sizeof(day))) return false;

    const auto* monthBytes = reinterpret_cast<const std::byte*>(&month);
    if (!file.write(reinterpret_cast<const char*>(monthBytes), sizeof(month))) return false;

    const auto* yearBytes = reinterpret_cast<const std::byte*>(&year);
    if (!file.write(reinterpret_cast<const char*>(yearBytes), sizeof(year))) return false;
    return file.good();
}

bool Database::writeDouble(double value) {
    const auto* bytes = reinterpret_cast<const std::byte*>(&value);
    if (!file.write(reinterpret_cast<const char*>(bytes), sizeof(value))) return false;
    return file.good();
}

bool Database::writeSize(size_t value) {
    const auto* bytes = reinterpret_cast<const std::byte*>(&value);
    if (!file.write(reinterpret_cast<const char*>(bytes), sizeof(value))) return false;
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
    auto* dayBytes = reinterpret_cast<std::byte*>(&day);
    file.read(reinterpret_cast<char*>(dayBytes), sizeof(day));

    auto* monthBytes = reinterpret_cast<std::byte*>(&month);
    file.read(reinterpret_cast<char*>(monthBytes), sizeof(month));

    auto* yearBytes = reinterpret_cast<std::byte*>(&year);
    file.read(reinterpret_cast<char*>(yearBytes), sizeof(year));
    return Date(day, month, year);
}

double Database::readDouble() {
    double value;
    auto* bytes = reinterpret_cast<std::byte*>(&value);
    file.read(reinterpret_cast<char*>(bytes), sizeof(value));
    return value;
}

size_t Database::readSize() {
    size_t value;
    auto* bytes = reinterpret_cast<std::byte*>(&value);
    file.read(reinterpret_cast<char*>(bytes), sizeof(value));
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
        throw DatabaseReadException("Failed to read transaction type");
    }
    

    auto id = readSize();
    if (!file.good()) {
        throw DatabaseReadException("Failed to read transaction ID");
    }
    

    auto name = readString();
    auto category = readString();
    auto date = readDate();
    auto amount = readDouble();
    
    if (!file.good()) {
        throw DatabaseReadException("Failed to read transaction basic data");
    }

    if (type == 0) {
        auto incomeSource = readString();
        if (!file.good()) {
            throw DatabaseReadException("Failed to read income source");
        }
        return std::static_pointer_cast<Transaction>(std::make_shared<IncomeTransaction>(id, name, category, date, amount, incomeSource));
    } else {
        auto where = readString();
        if (!file.good()) {
            throw DatabaseReadException("Failed to read expense location");
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

        auto pos = file.tellg();
        int test_type;
        auto* typeBytes = reinterpret_cast<std::byte*>(&test_type);
        file.read(reinterpret_cast<char*>(typeBytes), sizeof(test_type));
        
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
        } catch (const DatabaseReadException& e) {
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