#pragma once
#include "Shape.h"

class Rectangle : public Shape {
private:
    double width;
    double height;

public:
    Rectangle(double width, double height);
    ~Rectangle() override = default;

    double area() const override;
    void print() const override;
};
