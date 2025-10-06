#ifndef HEADER_H
#define HEADER_H

#include <conio.h>
#include <climits>
#include <iostream>
#include <string>
#include "Shape.h"

using namespace std;

bool checkIntToValid(const string& input, int min, int max, long long& out);

int secureInputMethod(int min, int max);

char chooseTaskNtoM(char n, char m);

void printMenu();

Shape* chooseFigure(const string& title);

void testArray();

void addShape(Shape** shapes, int& size, int capacity);

void showAllShapes(Shape** shapes, int size);

void showShapeByIndex(Shape** shapes, int size);

#endif
