#ifndef HANDMADEEXCEPTIONS_H
#define HANDMADEEXCEPTIONS_H

#include <iostream>
#include <stdexcept>
#include <string>

class SafeArray {
private:
    int *data = nullptr;
    int arrSize;

public:
    explicit SafeArray();

    explicit SafeArray(int size);

    SafeArray(const SafeArray &other);

    ~SafeArray();

    SafeArray &operator=(const SafeArray &other);

    const int &operator[](int index) const;

    int &operator[](int index);

    int size() const;
};

class OutOfRangeError : public std::exception {
private:
    int index;
    int size;
    bool isBelow;
    std::string message;

public:
    OutOfRangeError(int idx, int sz, bool below): index(idx), size(sz), isBelow(below){
        if (isBelow) {
            message = "ќшибка: индекс " + std::to_string(index) +
                       " меньше нул€ (нижн€€ граница массива).";
        }
        else {
            message = "ќшибка: индекс " + std::to_string(index) +
                       " превышает верхнюю границу массива (размер = " +
                       std::to_string(size) + ").";
        }
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};


#endif
