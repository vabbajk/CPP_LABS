#pragma once

template <typename T>
struct Node;

template <typename T>
class Queue;

template <typename T>
class QueueAlgorithms;

template <typename T>
class QueueIterator {
private:
    Node<T>* current;

public:
    explicit QueueIterator(Node<T>* ptr) : current(ptr) {}

    QueueIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }

    T& operator*() const { return current->data; }

    bool operator!=(const QueueIterator& other) const {
        return current != other.current;
    }

    friend class Queue<T>;
    friend class QueueAlgorithms<T>;
};

