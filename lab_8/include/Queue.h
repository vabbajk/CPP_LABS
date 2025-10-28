#pragma once
#include <iostream>
#include <stdexcept>
#include "QueueIterator.h"

template <typename T>
struct Node {
    T data;
    Node* next;
    explicit Node(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* rear;
    size_t count;

public:
    Queue() : front(nullptr), rear(nullptr), count(0) {}
    ~Queue() {
        while (!empty()) pop();
    }

    void push(const T& value) {
        Node<T>* node = new Node<T>(value);
        if (rear) rear->next = node;
        else front = node;
        rear = node;
        ++count;
    }

    void pop() {
        if (empty()) throw std::underflow_error("Очередь пуста");
        Node<T>* tmp = front;
        front = front->next;
        delete tmp;
        if (!front) rear = nullptr;
        --count;
    }

    bool empty() const { return count == 0; }

    size_t size() const { return count; }

    T& peek() {
        if (empty()) throw std::underflow_error("Очередь пуста");
        return front->data;
    }

    QueueIterator<T> begin() { return QueueIterator<T>(front); }
    QueueIterator<T> end() { return QueueIterator<T>(nullptr); }

    template <typename> friend class QueueAlgorithms;
    template <typename> friend class QueueIterator;
};
