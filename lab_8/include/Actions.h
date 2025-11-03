#pragma once
#include <iostream>
#include <functional>
#include <conio.h>
#include "Queue.h"
#include "QueueAlgorithms.h"


using namespace std;

void printMenu();
char chooseTaskNtoM(char n, char m);

template <typename Container>
void handleAddElement(Container& q) {
    typename Container::value_type val;
    cout << "¬ведите элемент: ";
    cin >> val;
    q.push(val);
    cout << "Ёлемент добавлен.\n";
}

template <typename Container>
void handleShowQueue(const Container& q) {
    if (q.empty()) {
        cout << "ќчередь пуста.\n";
        return;
    }

    cout << "Ёлементы очереди: ";
    for (const auto& value : q)
        cout << value << " ";
    cout << "\n";
}

template <typename Container, typename SortFunc>
void handleSortQueue(Container& q, SortFunc sorter) {
    if (q.empty()) {
        cout << "ќчередь пуста.\n";
        return;
    }

    QueueAlgorithms<typename Container::value_type>::sort(q, sorter);
    cout << "ќчередь отсортирована.\n";
}

template <typename Container, typename Value, typename Compare>
void handleFindElement(const Container& q, const Value& value, Compare equal) {
    if (q.empty()) {
        std::cout << "ќчередь пуста.\n";
        return;
    }

    bool found = false;
    for (const auto& elem : q) {
        if (equal(elem, value)) {
            found = true;
            break;
        }
    }

    if (found)
        std::cout << "Ёлемент найден.\n";
    else
        std::cout << "Ёлемент не найден.\n";
}

template <typename Container>
void handleFindMenu(const Container& q) {
    if (q.empty()) {
        cout << "ќчередь пуста.\n";
        return;
    }

    typename Container::value_type val;
    cout << "¬ведите значение дл€ поиска: ";
    cin >> val;

    handleFindElement(q, val, [](const auto& a, const auto& b){ return a == b; });
}

template <typename Container>
void handleFindCase(const Container& q){
    cout << "=== ѕоиск элемента ===\n";
    handleFindMenu(q);
    cout << "======================\n";
}
