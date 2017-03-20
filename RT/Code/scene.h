//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "string.h"

typedef enum RenderMode {
    Phong,
    ZBuffer,
    Normal,
    Gooch,
} RenderMode;

inline const char* toString (RenderMode r)
{
    switch (r)
    {
        case Phong:     return "Phong";
        case ZBuffer:   return "z-buffer";
        case Normal:    return "normal buffer";
        case Gooch:     return "Gooch";
    }
}

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    Triple center;
    Triple up;
    RenderMode rm;
    bool shadows;
    int recursionDepth;
    int superSampling;
    float b;
    float y;
    float alpha;
    float beta;
public:
    Color trace(const Ray &ray, RenderMode rm, int depth);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setCenter (Triple c);
    void setUp (Triple u);
    void setRenderMode (string in);
    void setShadows (bool val);
    void setB(float val);
    void setY(float val);
    void setAlpha(float val);
    void setBeta(float val);
    void setMaxRecursionDepth (int val);
    void setSuperSampling(int val);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
