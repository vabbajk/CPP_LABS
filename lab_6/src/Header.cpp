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
    cout << "������������ ������ �6 - ����������" << endl;
    cout << "����:" << endl;
    cout << "1) �������� ������� � ������ �� �������" << endl;
    cout << "2) ������� �������� �������" << endl;
    cout << "3) ��������� ������� �� ������� (���� try-catch)" << endl;
    cout << "4) ����������� ������ (������ ������ ����� �������)" << endl;
    cout << "0) �����" << endl;
    cout << "��� �����: ";
}

void addByIndex(SafeArray &arr) {
    cout << "������� ������ (����� ���� ��� ������ �������): ";
    int idx = secureInputMethod(INT_MIN + 1, INT_MAX);
    if (idx == INT_MIN) {
        cout << "������." << endl;
        return;
    }
    cout << "������� �������� ��������: ";
    int value = secureInputMethod(INT_MIN + 1, INT_MAX);
    if (value == INT_MIN) {
        cout << "������." << endl;
        return;
    }
    try {
        arr[idx] = value;
        cout << "����������� arr[" << idx << "] = " << value << endl;
    } catch (const out_of_range &e) {
        cerr << "����������: " << e.what() << endl;
    }
}

void printArray(const SafeArray &arr) {
    if (arr.size() == 0) {
        cout << "������ ���� (������ 0)." << endl;
        return;
    }
    cout << "���������� ������� (������: " << arr.size() << "):" << endl;
    for (size_t i = 0; i < arr.size(); ++i) {
        try {
            cout << "[" << i << "] = " << arr[i] << '\n';
        } catch (const out_of_range &e) {
            cerr << "���������� ��� ������� � ������� " << i << ": " << e.what() << endl;
        }
    }
}

void readByIndex(const SafeArray &arr) {
    if (arr.size() == 0) {
        cout << "������ ���� (������ 0)." << endl;
        return;
    }
    cout << "������� ������ (����� ���� ��� ������ �������): ";
    int idx = secureInputMethod(INT_MIN + 1, INT_MAX);
    if (idx == INT_MIN) {
        cout << "������." << endl;
        return;
    }
    try {
        cout << "arr[" << idx << "] = " << arr[idx] << endl;
    } catch (const out_of_range &e) {
        cerr << "����������: " << e.what() << endl;
    }
}

void recreateArray(SafeArray &arr) {
    cout << "������� ����� ������ �������: ";
    int newSize = secureInputMethod(0, INT_MAX);
    if (newSize == INT_MIN) {
        cout << "������." << endl;
        return;
    }
    SafeArray tmp(newSize);
    arr = tmp;
    cout << "������ ����������. ����� ������: " << arr.size() << endl;
}
