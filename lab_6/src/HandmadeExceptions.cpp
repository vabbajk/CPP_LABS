#include "../include/HandmadeExceptions.h"

#include <format>
using namespace std;

 SafeArray::SafeArray() : data(nullptr), arrSize(0){}

SafeArray::SafeArray(int size) : arrSize(size) {
    if (size == 0) {
        data = nullptr;
    } else {
        data = new int[size]();
    }
}

SafeArray::SafeArray(const SafeArray& other)
    : data(nullptr), arrSize(other.arrSize)
 {
     if (arrSize > 0) {
         data = new int[arrSize];
         std::copy(other.data, other.data + arrSize, data);
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
        for (int i = 0; i < other.arrSize; ++i) {
            data[i] = other.data[i];
        }
    }

    return *this;
}

const int& SafeArray::operator[](int index) const {
    if (index >= arrSize || index < 0) {
        throw out_of_range(
            format("Индекс {} выходит за пределы массива (размер: {})", index, arrSize)
        );
    }
    return data[index];
}

int& SafeArray::operator[](int index) {
    if (index >= arrSize || index < 0) {
        throw out_of_range(
            format("Индекс {} выходит за пределы массива (размер: {})", index, arrSize)
        );
    }
    return data[index];
}

int SafeArray::size() const {
    return arrSize;
}