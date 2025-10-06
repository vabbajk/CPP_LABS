#pragma once
#include "Shape.h"

class Circle : public Shape {
private:
    double radius;

public:
    explicit Circle(double radius);
    ~Circle() override = default;

    double area() const override;
    void print() const override;
};
