
#include "torus.h"
#include <math.h>
#include "algebra/algebra.h"

Hit Torus::intersect (const Ray &ray)
{
	// Given ray, position, r, alpha
	// Torus equation:
	// 		(r - sqrt (x^2 + y^2))^2 + z^2 = alpha^2
	// =>
	// 		(r - sqrt (x^2 + y^2))^2 + z^2 - alpha^2 = 0

	Point ori = ray.O;
	Vector dir = ray.D;

	double a = 4 * r * r * (dir.x * dir.x + dir.y * dir.y);
	double b = 8 * r * r * (dir.x * (ori.x - position.x) + dir.y * (ori.y - position.y));
	double c = 4 * r * r * ((ori.x - position.x) * (ori.x - position.x) + (ori.y - position.y) * (ori.y - position.y));
	double d = dir.dot (dir);
	double e = 2 * (ori - position).dot (dir);
	double f = (ori - position).dot ((ori - position)) + r * r - alpha * alpha;

	// Solve:
	// 		d^2 t^4 + 2de t^3 + (2df + e^2 - a) t^2 + (2ef - b) t + f^2 - c = 0
	// Gives max. 4 solutions.
	// Rewrite:
	// 		aa t^4 + bb t^3 + cc t^2 + dd t + ee = 0

	double aa = d * d;
	double bb = 2 * d * e;
	double cc = 2 * d * f + e * e - a;
	double dd = 2 * e * f - b;
	double ee = f * f - c;

	double roots[4];
	int numRoots = Algebra::SolveQuarticEquation (aa, bb, cc, dd, ee, roots);

	if (!numRoots)
		return Hit::NO_HIT ();

	double minimalT = roots[0];

	for (int i = 1; i < numRoots; i++)
		if (roots [i] < minimalT)
			minimalT = roots [i];

	Point hit = ori + minimalT * dir;
	Vector direction = hit - position;
	direction.z = 0;
	Point centre = position + direction.normalized () * r;

	return Hit (minimalT, (hit - centre).normalized ());

}
