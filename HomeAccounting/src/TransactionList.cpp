#include "../include/TransactionList.h"

TransactionList::TransactionList() : dbFilename("") {

}

TransactionList::TransactionList(const std::string& dbFilename) : dbFilename(dbFilename) {
    loadFromDatabase();
}