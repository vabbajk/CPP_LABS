#include "../include/Circle.h"
#include <cmath>
#include <iostream>
using namespace std;

Circle::Circle(double radius) : Shape("Круг"), radius(radius) {}

double Circle::area() const { return M_PI * radius * radius; }

void Circle::print() const { cout << getName() << " (Радиус = " << radius << ")"; }
