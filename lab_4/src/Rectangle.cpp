#include "../include/Rectangle.h"
#include <iostream>

using namespace std;

Rectangle::Rectangle(double width, double height)
    : Shape("�������������"), width(width), height(height) {}

double Rectangle::area() const {
    return width * height;
}

void Rectangle::print() const {
    cout << getName() << " (������ = " << width << ", ������ = " << height << ")";
}
