#include "../include/Header.h"

#include <iostream>
#include <limits>

using namespace std;

bool checkIntToValid(const string &input, int min, int max, long long &out) {
    if (input.empty())
        return false;

    int pos = 0;
    bool is_negative = false;

    if (input[0] == '-') {
        if (min >= 0)
            return false;
        is_negative = true;
        pos = 1;
        if (pos == input.size())
            return false;
    }

    long long result = 0;
    for (; pos < input.size(); ++pos) {
        if (input[pos] < '0' || input[pos] > '9')
            return false;
        int digit = input[pos] - '0';

        if (result > (LLONG_MAX - digit) / 10)
            return false;

        result = result * 10 + digit;
    }

    if (is_negative)
        result = -result;

    if (result < min || result > max)
        return false;

    out = result;
    return true;
}

int secureInputMethod(int min, int max) {
    string line;
    long long value = 0;

    while (true) {
        char key = _getch();

        switch (key) {
            case 27:
                return INT_MIN;

            case ' ':
                if (!line.empty() && line.back() != ' ' && line.back() != '-') {
                    line.push_back(' ');
                    cout << ' ';
                }
                break;

            case '-':
                if (line.empty() || line.back() == ' ') {
                    line.push_back('-');
                    cout << '-';
                }
                break;

            case '\b':
                if (!line.empty()) {
                    cout << "\b \b";
                    line.pop_back();
                }
                break;

            case '\r':
            case '\n':
                if (!line.empty() && line.back() != '-' && checkIntToValid(line, min, max, value)) {
                    cout << endl;
                    return value;
                }
                break;

            default:
                if (key >= '0' && key <= '9') {
                    line.push_back(key);
                    cout << key;
                }
                break;
        }
    }
}

char chooseTaskNtoM(char n, char m) {
    char c = 'l';
    while (c < n || c > m) {
        c = _getch();
    }
    return c;
}

void printMenu() {
    cout << "Лабораторная работа №7 - демонстрация" << endl;
    cout << "Меню:" << endl;
    cout << "1) Добавить запись о сотруднике" << endl;
    cout << "2) Показать все записи" << endl;
    cout << "3) Удалить файл с данными (для очистки)" << endl;
    cout << "0) Выход" << endl;
    cout << "Ваш выбор: ";
}

