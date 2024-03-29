//
//  Framework for a raytracer
//  File: triangle.h
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

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"

class Triangle : public Object
{
public:
    Triangle(Point p0, Point p1, Point p2) : p0(p0), p1(p1), p2(p2) {}

    virtual Hit intersect(const Ray &ray);
    virtual Color textureColor (const Point hit);

    const Point p0;
    const Point p1;
    const Point p2;
};

#endif
