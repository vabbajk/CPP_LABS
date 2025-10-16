#include "../include/HandmadeExceptions.h"

#include <algorithm>
#include <format>
#include <string>
using namespace std;

SafeArray::SafeArray() : arrSize(0) {}

SafeArray::SafeArray(int size) : arrSize(size) {
    if (size <= 0) {
        data = nullptr;
        arrSize = 0;
    } else {
        data = new int[size]();
    }
}

SafeArray::SafeArray(const SafeArray &other) : arrSize(other.arrSize) {
    if (arrSize > 0) {
        data = new int[arrSize];
        std::copy(other.data, other.data + arrSize, data);
    }
}

SafeArray::~SafeArray() { delete[] data; }

SafeArray &SafeArray::operator=(const SafeArray &other) {
    if (this == &other) {
        return *this;
    }

    delete[] data;
    data = nullptr;

    arrSize = other.arrSize;
    if (arrSize > 0) {
        data = new int[arrSize];
        for (int i = 0; i < arrSize; ++i) {
            data[i] = other.data[i];
        }
    }

    return *this;
}

const int& SafeArray::operator[](int index) const {
    if (index < 0) {
        throw std::out_of_range(
            std::format("Индекс {} выходит за нижнюю границу массива ({} < 0)", index, index)
            );
    }
    if (index >= arrSize) {
        throw std::out_of_range(
            std::format("Индекс {} выходит за пределы массива (размер: {})", index, arrSize)
        );
    }
    return data[index];
}

int& SafeArray::operator[](int index) {
    if (index < 0) {
        throw std::out_of_range(
            std::format("Индекс {} выходит за нижнюю границу массива ({} < 0)", index, index)
            );
    }
    if (index >= arrSize) {
        throw std::out_of_range(
            std::format("Индекс {} выходит за пределы массива (размер: {})", index, arrSize)
        );
    }
    return data[index];
}

int SafeArray::size() const { return arrSize; }
