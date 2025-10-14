#pragma once
#include <iostream>
#include <string>

class Shape {
private:
    std::string name;

public:
    explicit Shape(const std::string &name);
    virtual ~Shape();

    virtual double area() const = 0;
    virtual void print() const = 0;
    const std::string &getName() const;
};
