#pragma once
#include <utility>
#include <functional>
#include "Queue.h"

template <typename T>
class QueueAlgorithms {
public:
    static Node<T>* find(Queue<T>& queue, const T& value) {
        for (auto it = queue.begin(); it != queue.end(); ++it)
            if (*it == value)
                return it.current;
        return nullptr;
    }

    static void sort(Queue<T>& queue, std::function<bool(const T&, const T&)> comp = std::less<T>()) {
        if (queue.size() < 2) return;
        bool swapped;
        do {
            swapped = false;
            for (Node<T>* i = queue.front; i && i->next; i = i->next) {
                if (!comp(i->data, i->next->data)) {
                    std::swap(i->data, i->next->data);
                    swapped = true;
                }
            }
        } while (swapped);
    }
};
