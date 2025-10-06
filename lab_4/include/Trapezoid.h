#pragma once
#include "Shape.h"

class Trapezoid : public Shape {
private:
    double baseA;
    double baseB;
    double height;

public:
    Trapezoid(double baseA, double baseB, double height);
    ~Trapezoid() override = default;

    double area() const override;
    void print() const override;
};
