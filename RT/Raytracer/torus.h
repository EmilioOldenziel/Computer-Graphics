

#ifndef TORUS_H
#define TORUS_H

#include "Object.h"

class Torus : public Object
{
public:
	Torus (Point position, double r, double alpha) : position (position), r (r), alpha (alpha) { }

	virtual Hit intersect (const Ray &ray);

	const Point position;	// Position of centre of the torus.
	const double r;			// Distance from centre of tube to centre of torus.
	const double alpha;		// Radius of the tube.
};

#endif
