#include <iomanip>
#include <iostream>
#include "Array.h"

#include "Header.h"

using namespace std;

int MyArray::allocate_memory() {
    if (size <= 0) {
        data = nullptr;
        return -1;
    }

    data = new (std::nothrow) int[size];
    if (data == nullptr) {
        std::cerr << "������ ��������� ������!" << std::endl;
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

void MyArray::print() const {
    if (data == nullptr || size == 0) {
        std::cout << "������ ����! " << std::endl;
        return;
    }
    
    for (int i = 0; i < size; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

int MyArray::append(int value) {

    auto temp = new int[size + 1];

    if (temp==nullptr) {
        exit(1);
    }

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

int MyArray::get_size() const {
    return size;
}

void input_array(MyArray & array) {
    cout << "������� ���������� ����������� ���������: ";

    int array_size = new_input_metod(0, INT_MAX);
    int temp;

    for (int i = 0; i < array_size; i++) {
        cout << i+1 << " > ";
        temp = new_input_metod(INT_MIN, INT_MAX);
        array.append(temp);
    }

}


