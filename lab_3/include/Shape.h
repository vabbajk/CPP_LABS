#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>

class Circle;
class Square;
class Triangle;

class Shape {
public:
    virtual ~Shape() = default;


    virtual bool intersect(Shape* other) = 0;


    virtual bool intersectWithCircle(Circle* other) = 0;
    virtual bool intersectWithSquare(Square* other) = 0;
    virtual bool intersectWithTriangle(Triangle* other) = 0;
};

#endif
