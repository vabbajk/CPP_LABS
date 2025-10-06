#include "../include/Header.h"
#include "../include/Rectangle.h"
#include "../include/Circle.h"
#include "../include/RightAngledTriangle.h"
#include "../include/Trapezoid.h"

#include <iostream>
#include <limits>
#include <vector>

using namespace std;

bool checkIntToValid(const string& input, int min, int max, long long& out)
{
    if (input.empty())
        return false;

    int pos = 0;
    bool is_negative = false;

    if (input[0] == '-')
    {
        if (min >= 0)
            return false;
        is_negative = true;
        pos = 1;
        if (pos == input.size())
            return false;
    }

    long long result = 0;
    for (; pos < input.size(); ++pos)
    {
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

int secureInputMethod(int min, int max)
{
    string line;
    long long value = 0;

    while (true)
    {
        char key = _getch();

        switch (key)
        {
            case 27:
                return INT_MIN;

            case ' ':
                if (!line.empty() && line.back() != ' ' && line.back() != '-')
                {
                    line.push_back(' ');
                    cout << ' ';
                }
                break;

            case '-':
                if (line.empty() || line.back() == ' ')
                {
                    line.push_back('-');
                    cout << '-';
                }
                break;

            case '\b':
                if (!line.empty())
                {
                    cout << "\b \b";
                    line.pop_back();
                }
                break;

            case '\r':
            case '\n':
                if (!line.empty() && line.back() != '-' && checkIntToValid(line, min, max, value))
                {
                    cout << endl;
                    return static_cast<int>(value);
                }
                break;

            default:
                if (key >= '0' && key <= '9')
                {
                    line.push_back(key);
                    cout << key;
                }
                break;
        }
    }
}

char chooseTaskNtoM(char n, char m)
{
    char c = 'l';
    while (c < n || c > m)
    {
        c = _getch();
    }
    return c;
}

void printMenu() {
    cout << "Лабораторная работа №4\n";
    cout << "1) Добавить фигуру в массив\n";
    cout << "2) Вывести все фигуры и их площади\n";
    cout << "3) Выбрать элемент массива по индексу и вывести площадь\n";
    cout << "4) Демонстрация (готовый массив)\n";
    cout << "0) Выход\n";
    cout << "Ваш выбор: ";
}

static double getPositiveDouble(const string& prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (!(cin >> v) || v <= 0.0) {
            cout << "Ошибка ввода. Введите положительное число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        return v;
    }
}

Shape* chooseFigure(const string& title) {
    cout << "=== " << title << " ===\n";
    cout << "1) Прямоугольник\n";
    cout << "2) Круг\n";
    cout << "3) Прямоугольный треугольник\n";
    cout << "4) Трапеция\n";
    cout << "0) Отмена\n";
    cout << "Выберите тип фигуры: ";

    char choice;
    choice = chooseTaskNtoM('0', '4');
    cout << choice << endl;

    switch (choice) {
        case '1': {
            double w = getPositiveDouble("Введите ширину: ");
            double h = getPositiveDouble("Введите высоту: ");
            return new Rectangle(w, h);
        }
        case '2': {
            double r = getPositiveDouble("Введите радиус: ");
            return new Circle(r);
        }
        case '3': {
            double a = getPositiveDouble("Введите катет A: ");
            double b = getPositiveDouble("Введите катет B: ");
            return new RightAngledTriangle(a, b);
        }
        case '4': {
            double a = getPositiveDouble("Введите основание a: ");
            double b = getPositiveDouble("Введите основание b: ");
            double h = getPositiveDouble("Введите высоту h: ");
            return new Trapezoid(a, b, h);
        }
        case '0':
        default:
            return nullptr;
    }
}

void testArray() {
    const int capacity = 4;
    Shape* arr[capacity];
    int size = 0;
    arr[size++] = new Rectangle(2.0, 3.0);
    arr[size++] = new Circle(1.0);
    arr[size++] = new RightAngledTriangle(3.0, 4.0);
    arr[size++] = new Trapezoid(2.0, 4.0, 3.0);

    cout << "Демонстрационный массив указателей на Shape:\n";
    for (int i = 0; i < size; ++i) {
        cout << i+1 << ") ";
        arr[i]->print();
        cout << " -> S = " << arr[i]->area() << '\n';
    }

    for (int i = 0; i < size; ++i) delete arr[i];
}

void addShape(Shape** shapes, int& size, int capacity) {
    if (size >= capacity) {
        cout << "Массив переполнен.\n";
        system("pause");
        return;
    }
    Shape* fig = chooseFigure("Добавление фигуры");
    if (fig) {
        shapes[size++] = fig;
        cout << "Фигура добавлена.\n";
    } else {
        cout << "Отмена добавления.\n";
    }
    system("pause");
}

void showAllShapes(Shape** shapes, int size) {
    if (size == 0) {
        cout << "Массив пуст. Сначала добавьте фигуры.\n";
    } else {
        cout << "Содержимое массива фигур:\n";
        for (int i = 0; i < size; ++i) {
            cout << i + 1 << ") ";
            shapes[i]->print();
            cout << " -> S = " << shapes[i]->area() << '\n';
        }
    }
    system("pause");
}

void showShapeByIndex(Shape** shapes, int size) {
    if (size == 0) {
        cout << "Массив пуст. Сначала добавьте фигуры.\n";
        system("pause");
        return;
    }

    cout << "Введите индекс элемента (0.." << size-1 << "): ";
    int idx = secureInputMethod(0, size-1);
    if (idx == INT_MIN) {
        cout << "Отмена.\n";
        system("pause");
        return;
    }
    if (idx >= 1 && idx <= size) {
        Shape* s = shapes[idx - 1];
        s->print();
        cout << " -> S = " << s->area() << '\n';
    } else {
        cout << "Некорректный индекс.\n";
    }
    system("pause");
}

