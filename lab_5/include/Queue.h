#pragma once
#include <iostream>
using namespace std;


template <typename T>
struct Node {
    T data;
    Node* next;
    explicit Node(const T& value);
};


template <typename T>
class Queue {
private:
    Node<T>* front;
    Node<T>* rear;
    int count;

public:
    Queue();
    ~Queue();

    bool isEmpty() const;
    void enqueue(const T& value);
    T dequeue();
    T peek() const;
    void print() const;
};

#include "../include/Queue.tpp"
