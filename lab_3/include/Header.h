#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <climits>
#include <conio.h>
#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Triangle.h"

bool check_int_1(const std::string &input, int min, int max, long long &out);

int new_input_metod(int min, int max);

char choce_task_n_to_m(char n, char m);

void print_menu();

Shape* choose_figure(const std::string& displays_text);

#endif
