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

void printMainMenu() {
    cout << "\n====== МЕНЮ ОЧЕРЕДЕЙ ======\n";
    cout << "1) Целочисленная очередь\n";
    cout << "2) Строковая очередь\n";
    cout << "3) Вещественная очередь\n";
    cout << "0) Выход\n";
    cout << "Ваш выбор: ";
}

void printQueueMenu() {
    cout << "\n--- Действия над очередью ---\n";
    cout << "1) Добавить элемент\n";
    cout << "2) Удалить элемент\n";
    cout << "3) Просмотреть первый элемент\n";
    cout << "4) Вывести всю очередь\n";
    cout << "0) Назад\n";
    cout << "Ваш выбор: ";
}

void handleIntQueue(Queue<int>& q) {
    bool subRun = true;
    while (subRun) {
        system("cls");
        cout << "Работа с целочисленной очередью:\n";
        printQueueMenu();
        char choice = chooseTaskNtoM('0', '4');
        cout << choice << endl;

        switch (choice) {
            case '0':
                subRun = false;
                break;

            case '1': {
                cout << "Введите число для добавления: ";
                int val = secureInputMethod(INT_MIN+1, INT_MAX);
                if (val != INT_MIN) {
                    q.enqueue(val);
                    cout << "Добавлено!\n";
                } else cout << "Отмена ввода.\n";
                system("pause");
                break;
            }

            case '2':
                cout << "Удалено: " << q.dequeue() << endl;
                system("pause");
                break;

            case '3':
                cout << "Первый элемент: " << q.peek() << endl;
                system("pause");
                break;

            case '4':
                q.print();
                system("pause");
                break;
        }
    }
}

void handleStringQueue(Queue<string>& q) {
    bool subRun = true;
    while (subRun) {
        system("cls");
        cout << "Работа со строковой очередью:\n";
        printQueueMenu();
        char choice = chooseTaskNtoM('0', '4');
        cout << choice << endl;

        switch (choice) {
            case '0':
                subRun = false;
                break;

            case '1': {
                cout << "Введите строку: ";
                string val;
                getline(cin >> ws, val);
                q.enqueue(val);
                cout << "Добавлено!\n";
                system("pause");
                break;
            }

            case '2':
                cout << "Удалено: " << q.dequeue() << endl;
                system("pause");
                break;

            case '3':
                cout << "Первый элемент: " << q.peek() << endl;
                system("pause");
                break;

            case '4':
                q.print();
                system("pause");
                break;
        }
    }
}

void handleDoubleQueue(Queue<double>& q) {
    bool subRun = true;
    while (subRun) {
        system("cls");
        cout << "Работа с вещественной очередью:\n";
        printQueueMenu();
        char choice = chooseTaskNtoM('0', '4');
        cout << choice << endl;

        switch (choice) {
            case '0':
                subRun = false;
                break;

            case '1': {
                cout << "Введите число с плавающей точкой: ";
                string s;
                getline(cin >> ws, s);
                try {
                    double val = stod(s);
                    q.enqueue(val);
                    cout << "Добавлено!\n";
                } catch (...) {
                    cout << "Ошибка: некорректное число.\n";
                }
                system("pause");
                break;
            }

            case '2':
                cout << "Удалено: " << q.dequeue() << endl;
                system("pause");
                break;

            case '3':
                cout << "Первый элемент: " << q.peek() << endl;
                system("pause");
                break;

            case '4':
                q.print();
                system("pause");
                break;
        }
    }
}