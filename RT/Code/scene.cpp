//
//  Framework for a raytracer
//  File: scene.cpp
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

#include <limits>
#include <math.h>
#include <ctime>
#include "scene.h"
#include "material.h"

// method to give the color based on rendermode
Color Scene::trace(const Ray &ray, RenderMode rm, int depth)
{
	if (!depth) return Color (0.0, 0.0, 0.0);

	// Find hit object and distance
	Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit hit(objects[i]->intersect(ray));
		if (hit.t<min_hit.t) {
			min_hit = hit;
			obj = objects[i];
		}
	}

	// No hit? Return background color.
	if (!obj) return Color(0.0, 0.0, 0.0);

	Material *material = obj->material;            //the hit objects material
	Point hit = ray.at(min_hit.t);                 //the hit point
	Vector N = min_hit.N;                          //the normal at hit point
	Vector V = -ray.D;                             //the view vector


	/****************************************************
	* This is where you should insert the color
	* calculation (Phong model).
	*
	* Given: material, hit, N, V, lights[]
	* Sought: color
	*
	* Hints: (see triple.h)
	*        Triple.dot(Vector) dot product
	*        Vector+Vector      vector sum
	*        Vector-Vector      vector difference
	*        Point-Point        yields vector
	*        Vector.normalize() normalizes vector, returns length
	*        double*Color        scales each color component (r,g,b)
	*        Color*Color        dito
	*        pow(a,b)           a to the power of b
	****************************************************/
	Color color;

	// return the distance to hitpoint for the zbuffer
	if (rm == ZBuffer)
		return Color (min_hit.t, min_hit.t, min_hit.t);
	// return the color between 0 and 1
	else if (rm == Normal)
		return (N * 0.5) + 0.5;
	else if (!(rm == Gooch)){
		// Set colour to ambient light.
		if (!material->texture)
			color = material->color * material->ka;
		else
			color = material->ka * obj->textureColor (hit);
	}

	// For each light source.
	for (unsigned int i = 0; i < lights.size (); i++)
	{
		// Determine hit object only if shadows enabled.
		Object *tmp = NULL;
		Hit tmp_hit (std::numeric_limits<double>::infinity(),Vector());
		Vector L = (lights[i]->position - hit).normalized ();
		if (this->shadows)
		{
			// Compute ray to light source from hit point.
			Ray diffusedLightRay (hit, L);

			// Determine blocking object(s).
			for (unsigned int i = 0; i < objects.size(); ++i) {
				Hit hit(objects[i]->intersect(diffusedLightRay));
				if (hit.t<tmp_hit.t) {
					tmp_hit = hit;
					tmp = objects[i];
				}
			}
		}

		// Refraction ray.
		Vector R = 2 * (L.dot (N)) * N - L;

		// No blocking objects -> Determine colour.
		if (!tmp || tmp == obj || tmp_hit.t > (lights[i]->position - hit).length ()) 
		{
			if(rm == Gooch){
				Color goochkd = lights[i]->color * material->color * material->kd;
				Color cool = Color(0,0,1) * b + this->alpha*goochkd;
				Color warm = Color(1,1,0) * y + this->beta*goochkd;
				// I = kCool ∗ (1 − dot(N,L))/2 + kW arm ∗ (1 + dot(N,L))/2
				color = cool * (1 - N.dot(L))/2 + warm * (1 + N.dot(L))/2;
			}
			else{
				// Diffuse lighting.
				if (!material->texture)
					color += (lights[i]->color * material->color * (fmax (0, N.dot (L)) * material->kd));
				else
					color += (lights[i]->color * obj->textureColor (hit) * (fmax (0, N.dot (L)) * material->kd));
			}
			// Specular lighting.
			color += (lights[i]->color * material->ks * pow (fmax (0, R.dot (V)), material->n));
		}
	}

	// Determine reflection ray.
	Vector H = 2 * (V.dot (N)) * N - V;

	// Determine reflection colour.
	Ray refractionRay (hit, H);
	Color refractionColour = trace (refractionRay, rm, depth - 1);
	color += (refractionColour * material->ks);
	
	return color;
}

void Scene::render(Image &img)
{
	cout << "Rendermode:\t\t" 		<< toString (rm)	<< endl
		<< "Shadows:\t\t" 			<< shadows 			<< endl
		<< "Recursion depth:\t" 	<< recursionDepth 	<< endl
		<< "Super sampling:\t\t" 	<< superSampling 	<< endl
		<< "Rendering scene..." 						<< endl;

	// Timing stuff
	std::clock_t start = std::clock ();
	double duration = 0.0;
	cout.precision (9);

	// Here begins the interesting part
	int w = img.width();
	int h = img.height();
	double min = std::numeric_limits<double>::max (), max = std::numeric_limits<double>::min ();

	Triple eye_to_center = (center - eye).normalized();
	Triple right = (eye_to_center.cross(up).normalized()) * up.length();

	Point top_left = center + (up*h/2) - (right*w/2);

	// For each pixel.
	for (int y = 0; y < h; y++) 
	{
		for (int x = 0; x < w; x++) 
		{
			Color col(0.0,0.0,0.0);
			float span = 1.0 / (this->superSampling + 1.0);

			for(int row = 1; row != this->superSampling + 1; row++)
			{
				for(int column = 1; column != this->superSampling + 1; column++)
				{
					// Determine pixel location. Around the centre point, 
					// adjusted for size of the view in combination with pixel 
					// size. Also adjusted for super sampling.

					// from topleft to bottom right
					Point pixel = top_left 
						//in the width direction with ss
						+ (x * right + (column * span * right)) 
						//in the height direction with ss					
						- (y * up + (row * span * up));


					// Shoot ray & trace.
					Ray ray (eye, (pixel - eye).normalized());
					col += trace (ray, rm, this->recursionDepth);
				}
			}
			col /= superSampling * superSampling;

			if (rm != ZBuffer)
				col.clamp();
			else
			{
				// Find minimum and maximum for zbuffer.
				if (col.r > 0 && col.r < min)
					min = col.r;
				if (col.r > max)
					max = col.r;
			}
			img (x, y) = col;
		}

		// Prints the progress.
		if ((y + 1) * 20 % h == 0)
		{
			duration = (std::clock () - start) / (double) CLOCKS_PER_SEC;
			int percentage = (y + 1) * 100 / h;
			cout << percentage 
				<< "\% done. Estimated completion in "
				<< floor ((duration / percentage) * (100 - percentage))
				<< " seconds."
				<< endl;
		}
	}
	// If ZBuffer has been selected, scale the colours.
	if (rm == ZBuffer)
	{
		cout << "Normalising the result..." << endl;
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				Color col = img.get_pixel (x, y);
				if (!col.r) continue; // Colour is black (background).
				col.r = 1 - ((col.r - min) / (max - min));
				col.g = 1 - ((col.g - min) / (max - min));
				col.b = 1 - ((col.b - min) / (max - min));
				img (x, y) = col;
			}
		}
	}
}

void Scene::addObject(Object *o)
{
	objects.push_back(o);
}

void Scene::addLight(Light *l)
{
	lights.push_back(l);
}

void Scene::setEye(Triple e)
{
	eye = e;
}

void Scene::setCenter (Triple c)
{
	center = c;
}

void Scene::setUp (Triple u)
{
	up = u;
}

// Render modes can be zbuffer normal or phong (=default)
void Scene::setRenderMode (string in)
{
	if (!in.compare ("zbuffer"))
		rm = ZBuffer;
	else if (!in.compare ("normal"))
		rm = Normal;
	else if (!in.compare ("gooch"))
		rm = Gooch;
	else
		rm = Phong;
}

// Set shadows
void Scene::setShadows (bool val)
{
	this->shadows = val;
}

void Scene::setB(float val){
	this->b = val;
}


void Scene::setY(float val){
	this->y = val;
}


void Scene::setAlpha(float val){
	this->alpha = val;
}

void Scene::setBeta(float val){
	this->beta = val;
}

// Set maximum recursion depth.
void Scene::setMaxRecursionDepth (int val)
{
	this->recursionDepth = val;
}

// Set super sampling
void Scene::setSuperSampling (int val)
{
	this->superSampling = val;
}
