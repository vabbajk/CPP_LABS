#include "../include/Trapezoid.h"
#include <iostream>
using namespace std;
Trapezoid::Trapezoid(double baseA, double baseB, double height) :
    Shape("��������"), baseA(baseA), baseB(baseB), height(height) {}

double Trapezoid::area() const { return (baseA + baseB) * height / 2.0; }

void Trapezoid::print() const {
    cout << getName() << " (��������� A = " << baseA << ", ��������� B = " << baseB << ", ������ = " << height << ")";
}
