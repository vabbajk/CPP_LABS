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
    cout << "\n====== ���� �������� ======\n";
    cout << "1) ������������� �������\n";
    cout << "2) ��������� �������\n";
    cout << "3) ������������ �������\n";
    cout << "0) �����\n";
    cout << "��� �����: ";
}

void printQueueMenu() {
    cout << "\n--- �������� ��� �������� ---\n";
    cout << "1) �������� �������\n";
    cout << "2) ������� �������\n";
    cout << "3) ����������� ������ �������\n";
    cout << "4) ������� ��� �������\n";
    cout << "0) �����\n";
    cout << "��� �����: ";
}

void handleIntQueue(Queue<int>& q) {
    bool subRun = true;
    while (subRun) {
        system("cls");
        cout << "������ � ������������� ��������:\n";
        printQueueMenu();
        char choice = chooseTaskNtoM('0', '4');
        cout << choice << endl;

        switch (choice) {
            case '0':
                subRun = false;
                break;

            case '1': {
                cout << "������� ����� ��� ����������: ";
                int val = secureInputMethod(INT_MIN+1, INT_MAX);
                if (val != INT_MIN) {
                    q.enqueue(val);
                    cout << "���������!\n";
                } else cout << "������ �����.\n";
                system("pause");
                break;
            }

            case '2':
                cout << "�������: " << q.dequeue() << endl;
                system("pause");
                break;

            case '3':
                cout << "������ �������: " << q.peek() << endl;
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
        cout << "������ �� ��������� ��������:\n";
        printQueueMenu();
        char choice = chooseTaskNtoM('0', '4');
        cout << choice << endl;

        switch (choice) {
            case '0':
                subRun = false;
                break;

            case '1': {
                cout << "������� ������: ";
                string val;
                getline(cin >> ws, val);
                q.enqueue(val);
                cout << "���������!\n";
                system("pause");
                break;
            }

            case '2':
                cout << "�������: " << q.dequeue() << endl;
                system("pause");
                break;

            case '3':
                cout << "������ �������: " << q.peek() << endl;
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
        cout << "������ � ������������ ��������:\n";
        printQueueMenu();
        char choice = chooseTaskNtoM('0', '4');
        cout << choice << endl;

        switch (choice) {
            case '0':
                subRun = false;
                break;

            case '1': {
                cout << "������� ����� � ��������� ������: ";
                string s;
                getline(cin >> ws, s);
                try {
                    double val = stod(s);
                    q.enqueue(val);
                    cout << "���������!\n";
                } catch (...) {
                    cout << "������: ������������ �����.\n";
                }
                system("pause");
                break;
            }

            case '2':
                cout << "�������: " << q.dequeue() << endl;
                system("pause");
                break;

            case '3':
                cout << "������ �������: " << q.peek() << endl;
                system("pause");
                break;

            case '4':
                q.print();
                system("pause");
                break;
        }
    }
}