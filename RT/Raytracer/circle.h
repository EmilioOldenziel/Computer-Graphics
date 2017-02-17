//
//  Framework for a raytracer
//  File: plane.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef CIRCLE_H
#define CIRCLE_H

#include "object.h"


// An infinite plane consist of a center point 
// and a orientation which is equal to the normal
class Circle : public Object
{
public:
    Circle(Point center, Vector N, double radius) : center(center), N(N), radius(radius) { }

    virtual Hit intersect(const Ray &ray);

    const Point center;
    const Vector N;
    const double radius;
};

#endif
