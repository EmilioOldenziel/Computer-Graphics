//
//  Framework for a raytracer
//  File: raytracer.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef RAYTRACER_H_6GQO67WK
#define RAYTRACER_H_6GQO67WK

#include <iostream>
#include <string>
#include "triple.h"
#include "resolution.h"
#include "light.h"
#include "scene.h"
#include "yaml/yaml.h"

// typedef struct Resolution {
// 	int x;
// 	int y;
// } Resolution;

class Raytracer {
private:
    Scene *scene;

    // Couple of private functions for parsing YAML nodes
    Material* parseMaterial(const YAML::Node& node);
    Object* parseObject(const YAML::Node& node);
    Light* parseLight(const YAML::Node& node);
    Resolution parseResolution (const YAML::Node &node);
    void parseCamera (const YAML::Node &node);
    void parseSuperSampling (const YAML::Node &node);
    Object *parseSphere (const YAML::Node &node);
    void parseGooch (const YAML::Node &node);


public:
    Raytracer() { }

    bool readScene(const std::string& inputFilename);
    void renderToFile(const std::string& outputFilename);
    Resolution resolution;
};

#endif /* end of include guard: RAYTRACER_H_6GQO67WK */
