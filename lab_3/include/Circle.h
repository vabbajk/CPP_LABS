#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class Circle : public Shape {
public:
    bool intersect(Shape *other) override;

    bool intersectWithCircle(Circle *other) override;
    bool intersectWithSquare(Square *other) override;
    bool intersectWithTriangle(Triangle *other) override;
};

#endif
