//
//  Framework for a raytracer
//  File: sphere.h
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

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"

class Sphere : public Object
{
public:
    Sphere(Point position,double r, double angle, Vector axis) : position(position), r(r), angle(angle), axis(axis) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color textureColor (const Point hit);
    Point rotatePoint (Point in);

    const Point position;
    const double r;
    double angle;
	Vector axis;
};

#endif /* end of include guard: SPHERE_H_115209AE */
