#pragma once
#include <iostream>
#include <stdexcept>
#include "QueueIterator.h"

template <typename T>
struct Node {
    T data;
    Node* next = nullptr;
    explicit Node(const T& value) : data(value) {}
};

template <typename T>
class Queue {
private:
    Node<T>* front = nullptr;
    Node<T>* rear = nullptr;
    size_t count = 0;

public:

    using value_type = T;

    Queue() = default;

    Queue(const Queue& other) {
        for (Node<T>* cur = other.front; cur; cur = cur->next)
            push(cur->data);
    }

    ~Queue() noexcept {
        Node<T>* current = front;
        while (current) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        front = rear = nullptr;
        count = 0;
    }

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            for (Node<T>* cur = other.front; cur; cur = cur->next)
                push(cur->data);
        }
        return *this;
    }

    Queue(Queue&& other) noexcept
    : front(other.front), rear(other.rear), count(other.count) {
        other.front = other.rear = nullptr;
        other.count = 0;
    }

    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            clear();
            front = other.front;
            rear = other.rear;
            count = other.count;
            other.front = other.rear = nullptr;
            other.count = 0;
        }
        return *this;
    }

    void push(const T& value) {
        auto node = new Node<T>(value);
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

    QueueIterator<T> begin() const { return QueueIterator<T>(front); }
    QueueIterator<T> end() const { return QueueIterator<T>(nullptr); }

    template <typename> friend class QueueAlgorithms;
    template <typename> friend class QueueIterator;

    void clear() noexcept {
        Node<T>* current = front;
        while (current) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        front = rear = nullptr;
        count = 0;
    }
};
