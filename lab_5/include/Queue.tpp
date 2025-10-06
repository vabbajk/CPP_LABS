#ifndef QUEUE_TPP
#define QUEUE_TPP

#include "Queue.h"
#include <iostream>

template <typename T>
Node<T>::Node(const T& value) : data(value), next(nullptr) {}

template <typename T>
Queue<T>::Queue() : front(nullptr), rear(nullptr), count(0) {}

template <typename T>
Queue<T>::~Queue() {
    while (!isEmpty()) {
        dequeue();
    }
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return count == 0;
}

template <typename T>
void Queue<T>::enqueue(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    count++;
}

template <typename T>
T Queue<T>::dequeue() {
    if (isEmpty()) {
        cout << "Очередь пуста!\n";
        return T();
    }

    Node<T>* temp = front;
    T value = temp->data;
    front = front->next;
    delete temp;
    count--;

    if (isEmpty()) rear = nullptr;
    return value;
}

template <typename T>
T Queue<T>::peek() const {
    if (isEmpty()) {
        cout << "Очередь пуста!\n";
        return T();
    }
    return front->data;
}

template <typename T>
void Queue<T>::print() const {
    if (isEmpty()) {
        cout << "Очередь пуста!\n";
        return;
    }
    cout << "Содержимое очереди: ";
    Node<T>* current = front;
    while (current) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << "\n";
}

#endif