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
    * RT1.3: INTERSECTION CALCULATION
    *
    * Given: ray, center, N (orientation)
    * Sought: intersects? if true: *t
    ****************************************************/

    // P0 = position.
    // P = (x,y,z) = ray.O + ray.D * t
    // S = P - P0
    // S dot (N) == 0 => P on plane.

    double angle = ray.D.dot(N.normalized ());
    if (fabs(angle) > 0.0001l)
    {
        double t = (-N).dot (ray.O - center) / angle;
        if (t >= 0 && angle > 0.0001l) return Hit (t, (-N).normalized());
        if (t >= 0 && angle < -0.0001l) return Hit (t, N.normalized());
    }
    return Hit::NO_HIT();
}

