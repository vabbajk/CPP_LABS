#pragma once
#include <iterator>

template <typename T>
struct Node;

template <typename T>
class Queue;

template <typename T>
class QueueAlgorithms;

template <typename T>
class QueueIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;

private:
    Node<T>* current;

public:
    explicit QueueIterator(Node<T>* ptr) : current(ptr) {}

    reference operator*() const { return current->data; }
    pointer operator->() const { return &current->data; }

    QueueIterator& operator++() {
        if (current) current = current->next;
        return *this;
    }

    QueueIterator operator++(int) {
        QueueIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const QueueIterator& other) const = default;

    bool operator!=(const QueueIterator& other) const = default;

    friend class Queue<T>;
    friend class QueueAlgorithms<T>;
};
