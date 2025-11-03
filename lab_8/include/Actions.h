#pragma once
#include <iostream>
#include <functional>
#include <conio.h>
#include "Queue.h"
#include "QueueAlgorithms.h"


void printMenu();
char chooseTaskNtoM(char n, char m);

template <typename Container>
void handleAddElement(Container& q) {
    typename Container::value_type val;
    std::cout << "¬ведите элемент: ";
    std::cin >> val;
    q.push(val);
    std::cout << "Ёлемент добавлен.\n";
}

template <typename Container>
void handleShowQueue(const Container& q) {
    if (q.empty()) {
        std::cout << "ќчередь пуста.\n";
        return;
    }

    std::cout << "Ёлементы очереди: ";
    for (const auto& value : q)
        std::cout << value << " ";
    std::cout << "\n";
}

template <typename Container, typename SortFunc>
void handleSortQueue(Container& q, SortFunc sorter) {
    if (q.empty()) {
        std::cout << "ќчередь пуста.\n";
        return;
    }

    QueueAlgorithms<typename Container::value_type>::sort(q, sorter);
    std::cout << "ќчередь отсортирована.\n";
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
        std::cout << "ќчередь пуста.\n";
        return;
    }

    typename Container::value_type val;
    std::cout << "¬ведите значение дл€ поиска: ";
    std::cin >> val;

    handleFindElement(q, val, [](const auto& a, const auto& b){ return a == b; });
}

template <typename Container>
void handleFindCase(const Container& q){
    std::cout << "=== ѕоиск элемента ===\n";
    handleFindMenu(q);
    std::cout << "======================\n";
}
