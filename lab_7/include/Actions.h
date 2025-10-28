#ifndef ACTIONS_H
#define ACTIONS_H

#include <algorithm>
#include <iostream>
#include <string>
#include "../include/FileManager.h"
#include "../include/Person.h"
#include <string>

void handleAddRecord(const std::string &storageFilename);

void handleShowAll(const std::string &storageFilename);

void handleRemoveFile(const std::string &storageFilename);

#endif



