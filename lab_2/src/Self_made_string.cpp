#include <iomanip>
#include <iostream>
#include "../include/Self_made_string.h"
#include "../include/Header.h"

using namespace std;


int String::str_length(const char* s) {
    int len = 0;
    while (s && s[len] != '\0') len++;
    return len;
}

void String::str_copy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
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

String::String() {
    data = new char[1];
    data[0] = '\0';
    length = 0;
}

String::String(const char* s) {
    length = str_length(s);
    data = new char[length + 1];
    str_copy(data, s);
}

String::String(const String& other) {
    length = other.length;
    data = new char[length + 1];
    str_copy(data, other.data);
}

String::~String() {
    delete[] data;
}

bool operator!(const String& s) {
    return (s.length == 0 || s.data == nullptr);
}

String String::operator()(int start, int end) const {
    if (start < 0 || start >= length || end < start) {
        return String("");
    }

    char* buffer = new char[end-start + 1];
    for (int i = start; i <= end; i++) {
        buffer[i] = data[i];
    }
    buffer[end+1] = '\0';

    String result(buffer);
    delete[] buffer;
    return result;
}

char& String::operator[](int index) {
    return data[index];
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

bool operator==(const String& s1, const String& s2) {
    return String::str_equal(s1.data, s2.data);
}

ostream& operator<<(ostream& out, const String& s) {
    if (!s) {
        return out;
    }else {
        out << s.data;
        return out;
    }
}

istream& operator>>(istream& in, String& s) {
    char buffer[1024];
    in.getline(buffer, sizeof(buffer));
    delete[] s.data;
    s.length = String::str_length(buffer);
    s.data = new char[s.length + 1];
    String::str_copy(s.data, buffer);
    return in;
}



