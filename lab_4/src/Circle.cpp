#include "../include/Circle.h"
#include <cmath>
#include <iostream>
using namespace std;

Circle::Circle(double radius) : Shape("����"), radius(radius) {}

double Circle::area() const { return M_PI * radius * radius; }

void Circle::print() const { cout << getName() << " (������ = " << radius << ")"; }
