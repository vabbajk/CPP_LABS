#ifndef HEADER_H
#define HEADER_H
#include <conio.h>

#include <climits>
#include <iostream>
#include <string>

#include "Self_made_string.h"

bool check_int_1(const std::string &input, int min, int max, long long &out);

int new_input_metod(int min, int max);

char choce_task_n_to_m(char n, char m);

void print_menu_2();

String substr_menu(String s);

void input_string(String &my_string);

void print_string(const String &my_string);

void clear_string(String &my_string);

void check_empty(const String &my_string);

void print_substring(const String &my_string);

void print_char_by_index(const String &my_string);

void copy_string(const String &src, String &dest);

void print_temp_string(const String &temp_string);

void clear_temp_string(String &temp_string);

void run_menu();

#endif
