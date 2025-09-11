#include "../include/Header.h"

using namespace std;

bool check_int_1(const std::string &input, int min, int max, long long &out) {
    if (input.empty()) return false;

    int pos = 0;
    bool is_negative = false;

    if (input[0] == '-') {
        if (min >= 0) return false;
        is_negative = true;
        pos = 1;
        if (pos == input.size()) return false;
    }

    long long result = 0;
    for (; pos < input.size(); ++pos) {
        if (input[pos] < '0' || input[pos] > '9') return false;
        int digit = input[pos] - '0';

        if (result > (LLONG_MAX - digit) / 10) return false;

        result = result * 10 + digit;
    }

    if (is_negative) result = -result;

    if (result < min || result > max) return false;

    out = result;
    return true;
}

int new_input_metod(int min, int max) {
    std::string line;
    long long value = 0;

    while (true) {
        char key = _getch();

        switch (key) {
            case 27:
                return INT_MIN;

            case ' ':
                if (!line.empty() && line.back() != ' ' && line.back() != '-') {
                    line.push_back(' ');
                    std::cout << ' ';
                }
            break;

            case '-':
                if (line.empty() || line.back() == ' ') {
                    line.push_back('-');
                    std::cout << '-';
                }
            break;

            case '\b':
                if (!line.empty()) {
                    std::cout << "\b \b";
                    line.pop_back();
                }
            break;

            case '\r':
            case '\n':
                if (!line.empty() && line.back() != '-' &&
                    check_int_1(line, min, max, value)) {
                    std::cout << std::endl;
                    return static_cast<int>(value);
                    }
            break;

            default:
                if (key >= '0' && key <= '9') {
                    line.push_back(key);
                    std::cout << key;
                }
            break;
        }
    }
}

char choce_task_n_to_m(char n, char m)  {
    char c = 'l';
    while (c < n || c > m) {
        c = _getch();
    }
    return c;
}

void print_menu() {
    cout << "������������ ������ N2, ������� 2\n" << endl;
    cout << "������� :" << endl;
    cout << "1 ��� ����� ������" << endl;
    cout << "2 ��� ������ ������" << endl;
    cout << "3 ��� ������� ������" << endl;
    cout << "4 ��� �������� �� ������� ������" << endl;
    cout << "5 ��� ������ ���������" << endl;
    cout << "6 ��� ������ �������� �� �������" << endl;
    cout << "7 ��� ����������� ����������� ������ �� ��������� ������" << endl;
    cout << "8 ��� ������ ��������� ������" << endl;
    cout << "9 ��� ������� ��������� ������" << endl;
    cout << "0 ��� ������ \n > ";
}

String substr_menu(String s) {
    system("cls");
    int x,y;
    cout << "������� ����� ������� ��������� : ";
    x = new_input_metod(0, s.get_length()-1);
    cout << "������� ������ ������� ��������� : ";
    y = new_input_metod(x, s.get_length()-1);
    return s(x,y);
}