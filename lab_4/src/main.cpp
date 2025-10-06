#include <iostream>
#include "../include/Header.h"

using namespace std;

int main() {

    const int CAPACITY = 1024;

    system("chcp 1251");
    system("cls");

    Shape *shapes[CAPACITY] = {nullptr};
    int size = 0;
    bool running = true;

    while (running) {
        system("cls");
        printMenu();

        char choice = chooseTaskNtoM('0', '4');
        system("cls");

        span<Shape *> spanShapes(shapes, size);

        switch (choice) {
            case '1':
                addShape(shapes, size, CAPACITY);
                break;

            case '2':
                showAllShapes(spanShapes);
                break;

            case '3':
                showShapeByIndex(spanShapes);
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

    for (int i = 0; i < size; ++i) {
        delete shapes[i];
        shapes[i] = nullptr;
    }

    return 0;
}
