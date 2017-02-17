//
//  Framework for a raytracer
//  File: triangle.cpp
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

#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/

double abs (double x)
{
    return x < 0 ? -x : x;
}

Hit Triangle::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, p0, p1, p2
    * Sought: intersects? if true: *t
    ****************************************************/
    Vector e01 = p1 - p0;
    Vector e02 = p2 - p0;

    Vector N = e01.cross(e02);

    Vector Q = ray.D.cross(e02);

    double a = e01.dot(Q);

    if ((N.dot(ray.D) >= 0) || (abs(a) <= 0.001)) return Hit::NO_HIT();

    Vector s = (ray.O - p0) / a;
    Vector r = s.cross(e01);

    double k = s.dot(Q);
    double l = r.dot(ray.D);
    double m = 1.0f - k - l;

    //check if point is on the rectangle plane
    if ((k < 0.0f) || (l < 0.0f) || (m < 0.0f)) return Hit::NO_HIT();

    double t = e02.dot(r);
    
    if(t >= 0.0f){
        return Hit(t,N.normalized());
    }

    return Hit::NO_HIT();
}

