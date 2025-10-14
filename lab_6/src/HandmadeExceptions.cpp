#include "../include/HandmadeExceptions.h"
using namespace std;

 SafeArray::SafeArray() : arrSize(0) {
     data = nullptr;
}

SafeArray::SafeArray(size_t size) : arrSize(size) {
    if (size == 0) {
        data = nullptr;
    } else {
        data = new int[size]();
    }
}

SafeArray::~SafeArray() {
    delete[] data;
}

SafeArray &SafeArray::operator=(const SafeArray& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data;

    arrSize = other.arrSize;
    if (other.arrSize == 0) {
        data = nullptr;
    } else {
        data = new int[other.arrSize];
        for (size_t i = 0; i < other.arrSize; ++i) {
            data[i] = other.data[i];
        }
    }

    return *this;
}

const int & SafeArray::operator[](size_t index) const{
    if (index >= arrSize) {
        throw out_of_range(
            "Индекс " + to_string(index) +
            " выходит за пределы массива (размер: " +
            to_string(arrSize) + ")"
        );
    }
    return data[index];
}

int & SafeArray::operator[](size_t index) {
        if (index >= arrSize || index < 0) {
            throw out_of_range(
                "Индекс " + to_string(index) +
                " выходит за пределы массива (размер: " +
                to_string(arrSize) + ")"
            );
        }
        return data[index];
}

size_t SafeArray::size() const {
    return arrSize;
}