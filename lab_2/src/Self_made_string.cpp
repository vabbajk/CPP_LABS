#include <iomanip>
#include <iostream>
#include "../include/Self_made_string.h"
#include "../include/Header.h"

using namespace std;


int String::str_length(const char* s) {
    if (s == nullptr) {
        return 0;
    }

    const char* ptr = s;
    while (*ptr != '\0') {
        ptr++;
    }
    return static_cast<int>(ptr - s);
}

void String::str_copy(char* dest, const char* src, int dest_size) {
    if (dest == nullptr || src == nullptr || dest_size == 0) {
        return;
    }

    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool String::str_equal(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) return false;
        i++;
    }
    return s1[i] == s2[i];
}

String::String() : data(new char[1]), length(0) {
    data[0] = '\0';
}

String::String(const char* s) : length(str_length(s)) {
    data = new char[length + 1];
    str_copy(data, s, length+1);
}

String::String(const String& other) : length(str_length(other.data)){
    data = new char[length + 1];
    str_copy(data, other.data, length+1);
}

String::~String() {
    delete[] data;
}

String String::operator()(int start, int end) const {
    if (start < 0 || start >= length || end < start || end >= length) {
        return String("");
    }

    int diff = end - start + 1;
    auto buffer = new char[diff + 1];

    for (int i = 0; i < diff; i++) {
        buffer[i] = data[start + i];
    }
    buffer[diff] = '\0';

    String result(buffer);
    delete[] buffer;
    return result;
}

const char& String::operator[](int index) const {
    return data[index];
}

String& String::operator=(const char* str) {
    *this = String(str);
    return *this;
}

String& String::operator=(const String& other){
    new (this) String(other);
    return *this;
}

int String::get_length() const{
    return length;
}

void String::clear() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
        length = 0;
        return;
    }
    else {
        return;
    }
}






