#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/

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

    //normal calculation
    Vector N = e01.cross(e02);

    Vector Q = ray.D.cross(e02);

    double a = e01.dot(Q);

    //check if triangle is not parallel with the ray
    if ((N.dot(ray.D) >= 0) || (fabs(a) <= 0.001)) 
        return Hit::NO_HIT();

    Vector s = (ray.O - p0) / a;
    Vector r = s.cross(e01);

    double k = s.dot(Q);
    double l = r.dot(ray.D);
    double m = 1.0f - k - l;

    //check if point is on the rectangle plane
    if ((k < 0.0f) || (l < 0.0f) || (m < 0.0f)) 
        return Hit::NO_HIT();

    double t = e02.dot(r);
    
    //check if triangle is not behind the camera
    if(t >= 0.0f){
        return Hit(t,N.normalized());
    }

    return Hit::NO_HIT();
}

