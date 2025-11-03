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

    template <typename Compare = std::less<T>>
static void sort(Queue<T>& queue, Compare comp = Compare()) {
        if (queue.size() < 2) return;

        bool swapped;
        do {
            swapped = false;
            for (Node<T>* i = queue.front; i && i->next; i = i->next) {
                if (comp(i->next->data, i->data)) {
                    std::swap(i->data, i->next->data);
                    swapped = true;
                }
            }
        } while (swapped);
    }
};
