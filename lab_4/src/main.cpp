#include <iostream>
#include "../include/Header.h"

#define CAPACITY 64

using namespace std;

int main() {
    system("chcp 1251");
    system("cls");

    Shape* shapes[CAPACITY];
    int size = 0;
    bool running = true;

    while (running) {
        system("cls");
        printMenu();
        char choice = chooseTaskNtoM('0', '4');

        system("cls");

        switch (choice) {
            case '1':
                addShape(shapes, size, CAPACITY);
                break;
            case '2':
                showAllShapes(shapes, size);
                break;
            case '3':
                showShapeByIndex(shapes, size);
                break;
            case '4':
                testArray();
                system("pause");
                break;
            case '0':
                running = false;
                break;
            default:
                cout << "Некорректный выбор\n";
                system("pause");
            break;
        }
    }

    for (int i = 0; i < size; ++i) delete shapes[i];

    return 0;
}
