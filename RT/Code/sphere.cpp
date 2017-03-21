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
#include "algebra/algebra.h"
#include <math.h>

/************************** Sphere **********************************/

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

	// Determine coefficients
	double a = 1;
	double b = 2 * (ray.O - position).dot (ray.D);
	double c = ((ray.O - position).dot (ray.O - position)) - (r * r);

	// Determine hit points (on the ray).
	double roots [2];
	int numRoots = Algebra::SolveQuadraticEquation (a, b, c, roots);

	// Determine closest hit point (if any).
	double t = 0;
	if (numRoots == 2){
		if(roots[0] > 0.00001 && roots[1] > 0.00001){
			t = min (roots[0], roots[1]);
		}
		else if(roots[0] > 0.00001 && roots[1] <= 0.00001){
			t = roots[0];
		}
		else if(roots[0] <= 0.00001 && roots[1] > 0.00001){
			t = roots[1];
		}
		else{
			return Hit::NO_HIT ();
		}
	}
		
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
	Vector N = ((ray.O + t*ray.D) - position).normalized ();

	return Hit(t,N);
}

Point Sphere::rotatePoint (Point in)
{
	const double pi = 3.14159265358979323846;
	Point out;
	double c = cos(angle / 180 * pi);
	double s = sin(angle / 180 * pi);
	double C = 1.0 - c;
	Vector a = axis.normalized ();

	double Q[3][3];
	Q[0][0] = c + a.x * a.x * C;
	Q[0][1] = a.x * a.y * C - a.z * s;
	Q[0][2] = a.x * a.z * C + a.y * s;

	Q[1][0] = a.x * a.y * C + a.z * s;
	Q[1][1] = c + a.y * a.y * C;
	Q[1][2] = a.y * a.z * C - a.x * s;

	Q[2][0] = a.x * a.z * C - a.y * s;
	Q[2][1] = a.y * a.z * C + a.x * s;
	Q[2][2] = c + a.z * a.z * C;

	out.x = in.x * Q[0][0] + in.x * Q[0][1] + in.x * Q[0][2];
	out.y = in.y * Q[1][0] + in.y * Q[1][1] + in.y * Q[1][2];
	out.z = in.z * Q[2][0] + in.z * Q[2][1] + in.z * Q[2][2];
	return out;
}

Color Sphere::textureColor (Point hit)
{
	Point tmp = rotatePoint (hit - position);
	const double pi = 3.14159265358979323846;
	float theta = acos ((float)(tmp.z) / (float)r);
	float phi   = atan2 ((float)(tmp.y), (float)(tmp.x));
	if (phi < 0)
		phi += 2 * pi;

	float u = phi / (2 * pi);
	float v = (pi - theta) / pi;
	return this->material->texture->colorAt (u, v);
}

