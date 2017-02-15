//
//  Framework for a raytracer
//  File: plane.cpp
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

#include "plane.h"
#include <iostream>
#include <math.h>

/************************** plane **********************************/


Hit Plane::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, center, N
    * Sought: intersects? if true: *t
    ****************************************************/

    float denom = N.dot(ray.D);
    if (fabs(denom) > 0.0001f) // your favorite epsilon
    {
        float t = (center - ray.O).dot(N) / denom;
        if (t >= 0) return Hit(t,N.normalized());
    }
    return Hit::NO_HIT();
}

