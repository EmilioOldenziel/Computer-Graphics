#ifndef PLANE_H
#define PLANE_H

#include "object.h"

// An infinite plane consist of a centre point and a normal.
class Plane : public Object
{
public:
    Plane(Point center, Vector N) : center(center), N(N) { }

    virtual Hit intersect(const Ray &ray);

    const Point center;	// Centre of the plane.
    const Vector N;		// Normal of the plane.
};

#endif
