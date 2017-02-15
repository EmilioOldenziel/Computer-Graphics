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

#ifndef plane_H
#define plane_H

#include "object.h"

class Plane : public Object
{
public:
    Plane(Point center, Vector N) : center(center), N(N) { }

    virtual Hit intersect(const Ray &ray);

    const Point center;
    const Vector N;
};

#endif
