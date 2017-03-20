
#include "torus.h"
#include <math.h>
#include <limits>
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

	// Determine coefficients.
	double aa = d * d;
	double bb = 2 * d * e;
	double cc = 2 * d * f + e * e - a;
	double dd = 2 * e * f - b;
	double ee = f * f - c;

	// Compute roots.
	double roots[4];
	int numRoots = Algebra::SolveQuarticEquation (aa, bb, cc, dd, ee, roots);

	// Find smallest root, if any.
	if (!numRoots)
		return Hit::NO_HIT ();
	double t = std::numeric_limits<double>::max ();
	for (int i = 0; i < numRoots; i++)
		if (roots [i] < t && roots[i] > 0.00001)
			t = roots [i];

	// Determine centre of the tube perpendicular to the hit point.
	Point hit = ori + t * dir;
	Vector direction = hit - position;
	direction.z = 0;
	Point centre = position + direction.normalized () * r;

	// Determine normal and return hit.
	return Hit (t, (hit - centre).normalized ());

}

Color Torus::textureColor (const Point hit)
{
    cerr << "Textures not supported for tori." << endl;
    return Color (0, 0, 0);
}
