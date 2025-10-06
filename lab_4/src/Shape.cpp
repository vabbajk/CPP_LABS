#include "../include/Shape.h"

using namespace std;

Shape::Shape(const string &name) : name(name) {}

Shape::~Shape() = default;

void Shape::print() const { cout << name; }

const string &Shape::getName() const { return name; }
