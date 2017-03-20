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
    // check if ray and plane are not parallel
    if (fabs(angle) > 0.0001l)
    {
        double t = (-N).dot (ray.O - center) / angle;
        //check if plane is not behind the camera and which side is shown
        if (t >= 0.00001 && angle > 0.00001) return Hit (t, (-N).normalized());
        if (t >= 0.00001 && angle < -0.00001) return Hit (t, N.normalized());
    }
    return Hit::NO_HIT();
}

Color Plane::textureColor (const Point hit)
{
    cerr << "Textures not supported for planes." << endl;
    return Color (0, 0, 0);
}

