//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/


int abc(double a, double b, double c, double* x1, double* x2){
    double discriminant  = (b*b) - (4 * a * c);
    if(discriminant > 0){
        *x1 = (-b + sqrt(discriminant))/2;
        *x2 = (-b - sqrt(discriminant))/2;
        return 2;
    }
    if(discriminant < 0){
        x1 = NULL;
        x2 = NULL;
        return 0;
    }
    // discriminant == 0
    *x1 = (-b + sqrt(discriminant))/2;
    x2 = NULL;
    return 1;
}


Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    // place holder for actual intersection calculation

    double a = 1;
    double b = 2 * (ray.O - position).dot (ray.D);
    double c = ((ray.O - position).dot (ray.O - position)) - (r * r);

    double x1 = 0;
    double x2 = 0;
    int cnt = abc (a, b, c, &x1, &x2);

    double t = 0;
    if (cnt == 2)
        t = min (x1, x2);
    else if (cnt == 1)
        t = x1;
    else
        return Hit::NO_HIT ();

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/


    Vector N = position - (ray.O + t*ray.D);

    return Hit(t,N);
}

