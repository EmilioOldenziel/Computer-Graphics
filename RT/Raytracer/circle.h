#ifndef CIRCLE_H
#define CIRCLE_H

#include "object.h"


// An infinite plane consist of a centre point and a normal. A circle adds a
// radius.
class Circle : public Object
{
public:
    Circle(Point center, Vector N, double radius) : center(center), N(N), radius(radius) { }

    virtual Hit intersect(const Ray &ray);

    const Point center;		// Centre of the circle.
    const Vector N;			// Normal of the plane in which the circle lays.
    const double radius;	// Radius of the circle.
};

#endif
