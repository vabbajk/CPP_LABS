#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H


#include <string>
#include <ostream>
#include "Person.h"


void append_record(const std::string &filename, const Person &p);


bool print_all(const std::string &filename, std::ostream &os);


bool remove_file(const std::string &filename);


#endif
