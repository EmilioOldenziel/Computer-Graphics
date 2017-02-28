#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"

// Cylinder is defined by a position, a unit vector indicating the direction of
// its centre axis, a height, and a radius.

class Cylinder : public Object
{
public:
	Cylinder (Point position, double r, double height, Vector direction) : position (position), r (r), height (height), direction(direction) { }

    virtual Hit intersect(const Ray &ray);

    const Point position;	// Position of the start of the centre axis.
    const double r;			// Radius of the cylinder.
    const double height;	// Length of the centre axis.
    const Vector direction;	// Direction of the centre axis.
};

#endif
