#ifndef HEADER_H
#define HEADER_H

#include "Shape.h"

#include <climits>
#include <conio.h>
#include <iostream>
#include <span>
#include <string>

bool checkIntToValid(const std::string &input, int min, int max, long long &out);

int secureInputMethod(int min, int max);

char chooseTaskNtoM(char n, char m);

void printMenu();

Shape *chooseFigure(const std::string &title);

void testArray();

void addShape(Shape **shapes, int &size, int capacity);

void showAllShapes(std::span<Shape *> shapes);

void showShapeByIndex(std::span<Shape *> shapes);

#endif
