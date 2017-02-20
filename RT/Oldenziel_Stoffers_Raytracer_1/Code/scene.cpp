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
#include "scene.h"
#include "material.h"

// method to give the color based on rendermode
Color Scene::trace(const Ray &ray, RenderMode rm)
{
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

    // return the distance to hitpoint for the zbuffer
    if (rm == ZBuffer)
        return Color (min_hit.t, min_hit.t, min_hit.t);
    // return the color between 0 and 1
    else if (rm == Normal)
        return (N * 0.5) + 0.5;

    // Set colour to ambient light.
    Color color = material->color * material->ka;

    // For each light source.
    for (unsigned int i = 0; i < lights.size (); i++)
    {
        // Compute ray to light source from hit point.
        Vector L = (lights[i]->position - hit).normalized ();
        Ray diffusedLightRay (hit, L);

        // Determine blocking object(s).
        Hit tmp_hit(std::numeric_limits<double>::infinity(),Vector());
        Object *tmp = NULL;
        for (unsigned int i = 0; i < objects.size(); ++i) {
            Hit hit(objects[i]->intersect(ray));
            if (hit.t<tmp_hit.t) {
                tmp_hit = hit;
                tmp = objects[i];
            }
        }

        Vector R = 2 * (L.dot (N)) * N - L;

        // No blocking objects -> Determine colour.
        if (!tmp || tmp == obj) 
        {
            // Diffuse lighting.
            color += lights[i]->color * material->color * (fmax (0, N.dot (L)) * material->kd);
            // Specular lighting.
            color += material->ks * pow (fmax (0, R.dot (V)), material->n);
        }
    }
    
    return color;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    double min = std::numeric_limits<double>::max (), max = std::numeric_limits<double>::min ();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray, rm);
            if (rm != ZBuffer)
                col.clamp();
            else
            {
                if (col.r > 0 && col.r < min)
                    min = col.r;
                if (col.r > max)
                    max = col.r;
            }
            img(x,y) = col;
        }
    }
    // If ZBuffer has been selected, scale the colours.
    if (rm == ZBuffer)
    {
        cerr << min << " " << max << endl;
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

//rendermodes can be zbuffer normal or phong (=default)
void Scene::setRenderMode (string in)
{
    if (!in.compare ("zbuffer"))
        rm = ZBuffer;
    else if (!in.compare ("normal"))
        rm = Normal;
    else
        rm = Phong;
}
