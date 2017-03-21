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

Point rotate (double mat[3][3], Point in)
{
	Point out;
	out.x = in.x * mat[0][0] + in.x * mat[0][1] + in.x * mat[0][2];
	out.y = in.y * mat[1][0] + in.y * mat[1][1] + in.y * mat[1][2];
	out.z = in.z * mat[2][0] + in.z * mat[2][1] + in.z * mat[2][2];
	return out;
}

Point Sphere::rotatePoint (Point in)
{
	const double pi = 3.14159265358979323846;
	Point out;
	Vector a = axis.normalized ();
	double alpha = acos (a.dot (Vector (1, 1, 0)));
	double c = cos (alpha);
	double s = sin (alpha);

	double Z[3][3];
	Z[0][0] = Z[1][1] = c;
	Z[0][1] = -s;
	Z[1][0] = s;
	Z[2][2] = 1;
	Z[2][0] = Z[2][1] = Z[0][2] = Z[1][2] = 0;

	out = rotate (Z, in);

	a = rotate (Z, a);
	alpha = acos (a.dot (Vector (1, 0, 0)));
	c = cos (alpha);
	s = sin (alpha);

	double Y[3][3];
	Y[0][0] = Y[2][2] = c;
	Y[0][2] = s;
	Y[2][0] = -s;
	Y[1][1] = 1;
	Y[1][0] = Y[0][1] = Y[1][2] = Y[2][1] = 0;

	out = rotate (Y, out);
	// cout << in.x << " " << in.y << " " << in.z << " -> " << out.x << " " << out.y << " " << out.z << " " << endl;

	alpha = angle / 180 * pi;
	c = cos (alpha);
	s = sin (alpha);

	double X[3][3];
	X[1][1] = X[2][2] = c;
	X[1][2] = -s;
	X[2][1] = s;
	X[0][0] = 1;
	X[0][1] = X[0][2] = X[1][0] = X[2][0] = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Z[i][i] = - Z[i][i];
			Y[i][i] = - Y[i][i];
		}
	}

	out = rotate (X, out);
	out = rotate (Y, out);
	out = rotate (Z, out);

	return out.normalized ();
}

Color Sphere::textureColor (Point hit)
{
	Point tmp = rotatePoint ((hit - position).normalized ()) * r;
	const double pi = 3.14159265358979323846;
	float theta = acos ((float)(tmp.z) / (float)r);
	float phi   = atan2 ((float)(tmp.y), (float)(tmp.x));
	if (phi < 0)
		phi += 2 * pi;

	float u = phi / (2 * pi);
	float v = (pi - theta) / pi;
	return this->material->texture->colorAt (u, v);
}

