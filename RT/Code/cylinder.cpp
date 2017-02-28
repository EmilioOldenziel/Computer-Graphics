#include <math.h>
#include "cylinder.h"
#include "circle.h"
#include "algebra/algebra.h"

Hit Cylinder::intersect (const Ray &ray)
{
	// Determine coefficients for cylinder equation.
	double a = 1 - ray.D.dot (direction) * ray.D.dot (direction);
	double b = 2 * ((ray.O - position).dot (ray.D) - 
		(ray.D.dot (direction) * (ray.O - position).dot (direction)));
	double c = (ray.O - position).dot (ray.O - position) - 
		(((ray.O - position).dot (direction)) * 
		((ray.O - position).dot (direction))) - (r * r);

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

	/****************************************************
	* RT1.2: NORMAL CALCULATION
	*
	* Given: t, C, r
	* Sought: N
	* 
	* Insert calculation of the sphere's normal at the intersection point.
	****************************************************/

	// Determine normal.
	double m = ray.D.dot (direction) * t + (ray.O - position).dot (direction);
	// The hit misses the cylinder. Maybe hits bottom cap?
	if (m < 0)
	{
		Circle *circle = new Circle (position, -direction, r);
		return circle->intersect (ray);
	}
	// The hit misses the cylinder. Maybe hits top cap?
	if (m > height)
	{
		Circle *circle = new Circle (position + direction * height, direction, r);
		return circle->intersect (ray);
	}

	// Determine normal.
	Vector N = ((ray.O + t*ray.D) - position - direction * m).normalized ();

	return Hit(t,N);  
}
