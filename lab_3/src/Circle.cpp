#include "../include/Circle.h"

bool Circle::intersect(Shape* other) {
    return other->intersectWithCircle(this);
}

bool Circle::intersectWithCircle(Circle* other) {
    std::cout << "Проверка пересечения Круг-Круг\n";
    return false;
}

bool Circle::intersectWithSquare(Square* other) {
    std::cout << "Проверка пересечения Круг-Квадрат\n";
    return false;
}

bool Circle::intersectWithTriangle(Triangle* other) {
    std::cout << "Проверка пересечения Круг-Треугольник\n";
    return false;
}
