#ifndef HANDMADEEXCEPTIONS_H
#define HANDMADEEXCEPTIONS_H

#include <string>
#include <iostream>
#include <stdexcept>

class SafeArray {
private:
    int* data;
    int arrSize;

public:

    explicit SafeArray();

    explicit SafeArray(int size);

    SafeArray(const SafeArray& other);

    ~SafeArray();

    SafeArray& operator=(const SafeArray& other);

    const int& operator[](int index) const ;

    int& operator[](int index);

    int size() const;
};

#endif
