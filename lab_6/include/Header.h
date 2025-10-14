#ifndef HEADER_H
#define HEADER_H

#include <climits>
#include <conio.h>
#include <iostream>
#include <string>
#include "HandmadeExceptions.h"

bool checkIntToValid(const std::string &input, int min, int max, long long &out);

int secureInputMethod(int min, int max);

char chooseTaskNtoM(char n, char m);

void printMenu();

void addByIndex(SafeArray &arr);

void printArray(const SafeArray &arr);

void readByIndex(const SafeArray &arr);

void recreateArray(SafeArray &arr);


#endif
