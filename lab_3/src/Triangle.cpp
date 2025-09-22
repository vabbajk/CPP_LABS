#include "../include/Triangle.h"

bool Triangle::intersect(Shape* other) {
    return other->intersectWithTriangle(this);
}

bool Triangle::intersectWithCircle(Circle* other) {
    std::cout << "Проверка пересечения Треугольник-Круг\n";
    return false;
}

bool Triangle::intersectWithSquare(Square* other) {
    std::cout << "Проверка пересечения Треугольник-Квадрат\n";
    return false;
}

bool Triangle::intersectWithTriangle(Triangle* other) {
    std::cout << "Проверка пересечения Треугольник-Треугольник\n";
    return false;
}
