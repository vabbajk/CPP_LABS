#include "../include/RightAngledTriangle.h"
#include <iostream>
using namespace std;

RightAngledTriangle::RightAngledTriangle(double legA, double legB)
    : Shape("������������� �����������"), legA(legA), legB(legB) {}

double RightAngledTriangle::area() const {
    return (legA * legB) / 2.0;
}

void RightAngledTriangle::print() const {
    cout << getName() << " (����� A= " << legA << ", ����� B= " << legB << ")";
}
