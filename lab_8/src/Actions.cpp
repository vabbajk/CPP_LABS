#include <iostream>
#include <conio.h>
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

void handleAddElement(Queue<int>& q) {
    int val;
    cout << "Введите число: ";
    cin >> val;
    q.push(val);
    cout << "Элемент добавлен.\n";
}

void handleShowQueue(Queue<int>& q) {
    if (q.empty()) {
        cout << "Очередь пуста.\n";
        return;
    }
    cout << "Элементы очереди: ";
    for (auto it = q.begin(); it != q.end(); ++it)
        cout << *it << " ";
    cout << "\n";
}

void handleSortQueue(Queue<int>& q) {
    if (q.empty()) {
        cout << "Очередь пуста.\n";
        return;
    }
    cout << "Выберите тип сортировки:\n"
            "1. По возрастанию\n"
            "2. По убыванию\n> ";
    char type;
    cin >> type;

    if (type == '1')
        QueueAlgorithms<int>::sort(q, [](int a, int b){ return a < b; });
    else
        QueueAlgorithms<int>::sort(q, [](int a, int b){ return a > b; });

    cout << "Очередь отсортирована.\n";
}

void handleFindElement(Queue<int>& q) {
    if (q.empty()) {
        cout << "Очередь пуста.\n";
        return;
    }
    int val;
    cout << "Введите значение для поиска: ";
    cin >> val;

    auto node = QueueAlgorithms<int>::find(q, val);
    if (node)
        cout << "Элемент " << val << " найден.\n";
    else
        cout << "Элемент " << val << " не найден.\n";
}
