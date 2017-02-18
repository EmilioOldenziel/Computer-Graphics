
#include <math.h>
#include "cylinder.h"
#include "circle.h"
#include "algebra/algebra.h"

Hit Cylinder::intersect (const Ray &ray)
{
	double a = ray.D.z * ray.D.z + ray.D.x * ray.D.x;
	double b = 2 * ((ray.O.z - position.z) * ray.D.z + (ray.O.x - position.x) * ray.D.x);
	double c = (ray.O.z - position.z) * (ray.O.z - position.z) + (ray.O.x - position.x) * (ray.O.x - position.x) - r * r;

	// Determine hit points (on the ray).
    double roots [2];
    int numRoots = Algebra::SolveQuadraticEquation (a, b, c, roots);

    // Determine closest hit point (if any).
    double t = 0;
    if (numRoots == 2)
        t = min (roots[0], roots[1]);
    else if (numRoots == 1)
        t = roots[0];
    else
        return Hit::NO_HIT ();

    Point hit = ray.O + ray.D * t;
    if (hit.y >= position.y && hit.y <= position.y + height)
    	return Hit(t, Vector ((ray.O + t*ray.D).x - position.x, 0, (ray.O + t*ray.D).z - position.z).normalized ());

	return Hit::NO_HIT ();    
}
