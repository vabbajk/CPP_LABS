#include "../include/Actions.h"
using namespace std;

void printMenu() {
    cout << "Лабораторная работа №8 - STL контейнеры\n";
    cout << "===== МЕНЮ ОЧЕРЕДИ =====\n";
    cout << "1. Добавить элемент\n";
    cout << "2. Показать очередь\n";
    cout << "3. Отсортировать очередь\n";
    cout << "4. Найти элемент\n";
    cout << "0. Выход\n";
    cout << "=========================\n";
    cout << "Выберите действие: ";
}

char chooseTaskNtoM(char n, char m) {
    char c = 'l';
    while (c < n || c > m) {
        c = _getch();
    }
    return c;
}



