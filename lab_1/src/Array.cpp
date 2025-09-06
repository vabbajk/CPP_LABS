#include <iomanip>
#include <iostream>
#include "Array.h"

#include "Header.h"

using namespace std;

int MyArray::allocate_memory() {
    data = new int[size];

    if (data == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        exit(1);
    }
    return 0;
}

int MyArray::free_memory() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    return 0;
}

MyArray::MyArray() : size(0), data(nullptr) {
}

MyArray::MyArray(int size)  : size(size) {
    allocate_memory();
}

MyArray::MyArray(const MyArray& other)
    : size(other.size),
      data(new int[size]) {

    for (int i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
}

MyArray::~MyArray() {
    if (data != nullptr) {
        free_memory();
    }
}

MyArray operator+(const MyArray& left, const MyArray& right) {
    MyArray result(left.size + left.size);

    for (int i = 0; i < left.size; i++) {
        result.data[i] = left.data[i];
    }

    for (int i = 0; i < right.size; i++) {
        result.data[left.size + i] = right.data[i];
    }

    return result;
}

MyArray operator&(const MyArray& a, const MyArray& b) {
    MyArray result;
    for (int i = 0; i < a.size; i++) {
        for (int j = 0; j < b.size; j++) {
            if (a.data[i] == b.data[j]) {
                result.append(a.data[i]);
                break;
            }
        }
    }
    return result;
}


MyArray& MyArray::operator=(const MyArray& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data;
    size = other.size;
    data = new int[size];

    for (int i = 0; i < size; i++) {
        data[i] = other.data[i];
    }

    return *this;
}

int MyArray::equalize(const int* arr, int other_size) {
    if (data != nullptr) {
        free_memory();
    }

    size = other_size;
    if (size > 0) {
        allocate_memory();
        for (int i = 0; i < size; ++i) {
            data[i] = arr[i];
        }
    } else {
        data = nullptr;
    }

    return 0;
}

const void MyArray::print() {
    if (data == nullptr || size == 0) {
        std::cout << "Массив пуст! " << std::endl;
        return;
    }
    
    for (int i = 0; i < size; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

int MyArray::append(int value) {

    auto temp = new int[size + 1];

    for (int i = 0; i < size; i++) {
        temp[i] = data[i];
    }
    temp[size] = value;

    if (data != nullptr) {
        delete[] data;
    }
    data = temp;
    size++;

    return 0;
}

void MyArray::clear() {
    delete[] data;
    data = nullptr;
    size = 0;
}

const int MyArray::get_size() {
    return size;
}

void input_array(MyArray & array) {
    cout << "Введите количество добавляемых элементов: ";

    int array_size = new_input_metod(1, INT_MAX);
    int temp;

    for (int i = 0; i < array_size; i++) {
        cout << i+1 << " > ";
        temp = new_input_metod(INT_MIN+1, INT_MAX);
        array.append(temp);
    }

}


