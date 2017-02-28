//
//  Framework for a raytracer
//  File: raytracer.cpp
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

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "torus.h"
#include "cylinder.h"
#include "circle.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    node["color"] >> m->color;	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);		
        returnObject = sphere;
    }
     
    if (objectType == "plane") {
        Point center;
        node["center"] >> center;
        Vector N;
        node["N"] >> N;
        Plane *plane = new Plane(center, N);		
        returnObject = plane;
    }

    //Added triangle support
    if (objectType == "triangle") {
        Point pos1;
        Point pos2;
        Point pos3;
        node["position1"] >> pos1;
        node["position2"] >> pos2;
        node["position3"] >> pos3;
        Triangle *triangle = new Triangle(pos1, pos2, pos3);		
        returnObject = triangle;
    }
    
    // Added support for tori.
    if (objectType == "torus") 
    {
        Point pos;
        double r, alpha;
        node["position"] >> pos;
        node["radius"] >> r;
        node["alpha"] >> alpha;
        Torus *torus = new Torus (pos, r, alpha);
        returnObject = torus;
    }

    // Added support for cylinders.
    if (objectType == "cylinder")
    {
        Point pos;
        double radius, height;
        Vector direction;
        node["position"] >> pos;
        node["radius"] >> radius;
        node["height"] >> height;
        node["direction"] >> direction;
        Cylinder *cylinder = new Cylinder (pos, radius, height, direction.normalized ());
        returnObject = cylinder;
    }

    // Added support for circles.
    if (objectType == "circle")
    {
        Point center;
        Vector N;
        double radius;
        node["center"] >> center;
        node["radius"] >> radius;
        node["N"] >> N;
        Circle *circle = new Circle (center, N, radius);
        returnObject = circle;
    }

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Read scene configuration options

            // Set render mode to specified value, default = phong.
            if (const YAML::Node *rec = doc.FindValue("RenderMode")) 
            { 
                string val;
                *rec >> val;
                scene->setRenderMode (val);
            }
            else
                scene->setRenderMode ("phong");

            // Set shadows to specified value, default = true.
            if (const YAML::Node *rec = doc.FindValue("Shadows")) 
            { 
                bool val;
                *rec >> val;
                scene->setShadows (val);
            }
            else
                scene->setShadows (true);

             // Set SuperSampling
            if (const YAML::Node *rec = doc.FindValue("SuperSampling")) 
            { 
                unsigned int val;
                *rec >> val;
                scene->setSuperSampling (val);
            }
            else
                scene->setSuperSampling (1);

            // Set MaxRecursionDepth to specified value, default = 1.
            if (const YAML::Node *rec = doc.FindValue("MaxRecursionDepth")) 
            { 
                unsigned int val;
                *rec >> val;
                scene->setMaxRecursionDepth (val);
            }
            else
                scene->setMaxRecursionDepth (1);

            scene->setEye(parseTriple(doc["Eye"]));

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(400,400);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
