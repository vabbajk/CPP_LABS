#pragma once
#include "Shape.h"

class RightAngledTriangle : public Shape {
private:
    double legA;
    double legB;
public:
    RightAngledTriangle(double legA, double legB);
    ~RightAngledTriangle() override = default;

    double area() const override;
    void print() const override;
};
