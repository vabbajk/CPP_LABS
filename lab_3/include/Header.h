#ifndef HEADER_H
#define HEADER_H

#include <climits>
#include <conio.h>
#include <iostream>
#include <string>
#include "Circle.h"
#include "Shape.h"
#include "Square.h"
#include "Triangle.h"

char choseTaskNtoM(char n, char m);

void printMenu();

Shape *chooseFigure(const std::string &displays_text);

#endif
