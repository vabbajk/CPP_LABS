#ifndef HEADER_H
#define HEADER_H

#include <climits>
#include <conio.h>
#include <iostream>
#include <string>
#include "../include/Queue.h"

bool checkIntToValid(const std::string &input, int min, int max, long long &out);

int secureInputMethod(int min, int max);

char chooseTaskNtoM(char n, char m);

void printMainMenu();

void printQueueMenu();

void handleIntQueue(Queue<int>& q);

void handleStringQueue(Queue<string>& q);

void handleDoubleQueue(Queue<double>& q);

#endif
