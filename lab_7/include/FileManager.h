#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <iostream>
#include <string>
#include <ostream>
#include "Person.h"

void append_record(const std::string &filename, const Person &p);

bool print_all(const std::string &filename, std::ostream &os);

bool remove_file(const std::string &filename);

class FileOpenException : public std::runtime_error {
public:
    explicit FileOpenException(const std::string& filename)
        : runtime_error("Ошибка при открытии файла: " + filename) {}
};




#endif
