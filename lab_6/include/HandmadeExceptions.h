#ifndef HANDMADEEXCEPTIONS_H
#define HANDMADEEXCEPTIONS_H

#include <string>
#include <iostream>
#include <stdexcept>

class SafeArray {
private:
    int* data;
    size_t arrSize;

public:

    explicit SafeArray();

    explicit SafeArray(size_t size);

    ~SafeArray();

    SafeArray& operator=(const SafeArray& other);

    const int& operator[](size_t index) const ;

    int& operator[](size_t index);

    size_t size() const;
};

#endif
