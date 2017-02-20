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

#include "circle.h"
#include <iostream>
#include <math.h>

/************************** plane **********************************/


Hit Circle::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.3: INTERSECTION CALCULATION
    *
    * Given: ray, center, N (orientation), radius
    * Sought: intersects? if true: *t
    ****************************************************/

    double angle = ray.D.dot(N.normalized ());
    // check if ray and plane are not parallel
    if (fabs(angle) > 0.0001f)
    {
        double t = (-N).dot (ray.O - center) / angle;
        //check if plane is not behind the camera
        if (t >= 0) 
        {
            Point hit = ray.O + t * ray.D;
            //plane is bound by the radius
            if ((hit - center).length () < radius)
                return Hit(t, (angle > 0.0001l ? -N : N).normalized());
        }
    }

    return Hit::NO_HIT();
}

