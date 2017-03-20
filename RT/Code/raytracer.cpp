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
#include "glm.h"

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
    // if(const YAML::Node *rec = node.FindValue ("texture")){
    //     cout << "text" << endl;
    //     std::string text;
    //     *rec >> text ;
    //     const char* c = text.c_str();
    //     m->texture = new Image(c);
    // }
    // else{
    //     node["color"] >> m->color;
    // }
    if (const YAML::Node *rec = node.FindValue("texture")) 
    { 
        std::string val;
        *rec >> val;
        m->texture = new Image(val.c_str ());
    }
    else
    {
        node["color"] >> m->color;
        m->texture = NULL;
    }

    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

void Raytracer::parseCamera (const YAML::Node &node)
{
    const YAML::Node& res = node["viewSize"];
    scene->setEye (parseTriple (node["eye"]));
    scene->setCenter (parseTriple (node["center"]));
    scene->setUp (parseTriple (node["up"]));
    unsigned int width, height;
    res[0] >> width;
    res[1] >> height;
    this->resolution = Resolution (width, height);
    cout << "Set resolution to " << width << " by " << height << endl;
}

void Raytracer::parseGooch (const YAML::Node &node)
{   
    float b;
    node["b"] >> b; 
    scene->setB(b);

    float y;
    node["y"] >> y;
    scene->setY(y);

    float alpha;
    node["alpha"] >> alpha;
    scene->setAlpha(alpha);

    float beta;
    node["beta"] >> beta;
    scene->setBeta(beta);
}

void Raytracer::parseSuperSampling (const YAML::Node &node)
{
    const YAML::Node& tmp = node["factor"];
    unsigned int val;
    tmp >> val;
    scene->setSuperSampling (val);
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

    // Added support for mesh.
    if (objectType == "mesh")
    {
        Point pos;
        float sf;
        node["position"] >> pos;
        std::string filename;
        node["filename"] >> filename;
        node["scalefactor"] >> sf;
        char* fn = &filename[0];
        GLMmodel *model = glmReadOBJ(fn);
        glmScale(model, sf);
        int num_triangles = model->numtriangles;
        cout << num_triangles << " triangles read" << endl;
        Material *material = parseMaterial(node["material"]);
        
        //add all triangles from model
        for(int i = 0; i < num_triangles; i++){
            //get triangle vertex indices
            int first = model->triangles[i].vindices[0];
            int second = model->triangles[i].vindices[1];
            int third = model->triangles[i].vindices[2];

            // cout << first << " " << second << " " << third << endl;

            // get triangle vertices
            Point pos1 = Point(
                model->vertices[3*first],
                model->vertices[3*first+1],
                model->vertices[3*first+2]) + pos;
            Point pos2 = Point(
                model->vertices[3*second],
                model->vertices[3*second+1],
                model->vertices[3*second+2]) + pos;
            Point pos3 = Point(
                model->vertices[3*third],
                model->vertices[3*third+1],
                model->vertices[3*third+2]) + pos;
                
            //make and add triangle
            Triangle *triangle = new Triangle(pos1, pos2, pos3);
            triangle->material = material;
            scene->addObject(triangle);
        }
        return NULL;
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

            if (const YAML::Node *rec = doc.FindValue("GoochParameters")){
                parseGooch (*rec);
            }

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
                parseSuperSampling (*rec);
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

            if (const YAML::Node *rec = doc.FindValue("Camera")) 
                parseCamera (*rec);
            else
            {
                scene->setEye(parseTriple(doc["Eye"]));
                this->resolution = Resolution ();
                scene->setCenter(Triple(this->resolution.width / 2, this->resolution.height / 2,0));
                scene->setUp(Triple(0,1,0));
            }

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
    Image img(this->resolution.width, this->resolution.height);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
