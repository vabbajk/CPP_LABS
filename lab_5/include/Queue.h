#pragma once
#include <iostream>


template<typename T>
struct Node {
    T data;
    Node *next;
    explicit Node(const T &value);
};


template<typename T>
class Queue {
private:
    Node<T> *front;
    Node<T> *rear;
    int count;

public:
    Queue();
    ~Queue();

    Queue(const Queue &other);
    Queue &operator=(const Queue &other);

    Queue(Queue &&other) noexcept;
    Queue &operator=(Queue &&other) noexcept;

    bool isEmpty() const;
    void enqueue(const T &value);
    T dequeue();
    T peek() const;
    void print() const;
};

#include "../include/Queue.tpp"
