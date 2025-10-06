#pragma once
#include <string>
#include <iostream>

using namespace std;

class Shape {
protected:
    string name;
public:
    Shape(const string& name);
    virtual ~Shape();

    virtual double area() const = 0;
    virtual void print() const;
    const string& getName() const;
};
