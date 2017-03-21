Authors: Emilio Oldenziel s2509679 & Yannick Stoffers s2372061
Group: Tuesday 02

Raytracer 3:
    3.1: Textures
        Spheres can now have textures. The location in the texture is determined
        by the position at which the ray hits the sphere using some rather 
        straightforward mathematics.

    3.2: Rotation
        We rotate a sphere by determining the rotation matrix around the given
        axis and the given angle. We rotate the hit point in 3D space and then 
        determine the corresponding texture coordinates. But, somewhere somehow 
        this doesn't work.

    3.3: Gooch
        For the gooch shading we had to change the lighting model a bit,
        If the rendermode is gooch we took out the ambient lighting and the 
        diffuse light was changed to the one from the paper. We calculated the 
        warm and cold color using y and b intensities and alpha and beta.

    3.4: Geometries
        -   We already had planes, triangles, cilinders and tori. 
    3.5:
        -   We read the filename from the yaml file, then we used the provided
            glm to read the .obj file. (we needed to install the freeglut-dev 
            lib to compile the glm file). After this we created a raytracer 
            triangle for every traingle in the mesh model with the material 
            from the yaml file. As example we took the cat from the OpenGL part.
            See mesh.yaml for an example, as a bonus we introduced scaling 
            factor of a mesh in the yaml file (see mesh.yaml).
            (See Screenshots for results).

Raytracer 2:
    2.1: Optical laws
        -   We made some changes in scene for the optical laws
        2.1.1: Shadows
            -   Last time we already implemented this, but apparently passed the 
                wrong variable as parameter. So, we changed the given parameter 
                in and it worked.
        2.1.2: Multiple light sources
            -   We already took this into account last time.
        2.1.3: Reflections
            -   The maximum recursion depth is read from the yaml file and 
                passed as a parameter to the trace function.
            -   In each "iteration" we determine the refraction ray H and 
                continue tracing with H from the hit point until the maximum 
                recursion depth is reached.

    2.2: Camera
        2.2.1: Super sampling
            -   first we read the super sampling factor n from the yaml file
                this one will be used to take n^2 samples from the a grid setup.
                after this the samples are averaged and used to determine the 
                colour.

        2.2.2: Camera extension
            -   First we read the camera from the yaml file (eye if not defined)
                and use the eye and center parameters to determine the outgoing
                rays. We compensate the destinations of the rays with the size
                of a pixel (determined by the up parameter) and size of the 
                view.

Comments:
-   All mentioned yaml files can found in the Scenes subdirectory. The 
    corresponding output images have been included in the Screenshots 
    subdirectory. 
-   We added a neat feature to the program. The progress of tracing is printed
    after every 5% of completion, including the estimated completion time.


Raytracer 1:
    1.1:
        1.1.1: Intersect Sphere
            We implemented sphere intersection in the sphere class.
            For the intersection of a ray and sphere we had to use the
            abc formula. 

        1.1.2: Normal Calculation
            The normal is the vector which is orthogonal to the surface of the 
            hit point.

        1.1.3: Phong
            We implemented the Phong illumination in the scene. We added 
            ambient, diffuse and specular lighting from each light source.

    1.2:
        First we introduced 3 rendermodes in the yaml file: 
        zbuffer,normal and phong. If no rendermode is give phong wil be used as 
        default.

        1.2.1: Z-buffer
            For the z-buffer we changed the trace class to only return
            the distance to the hit point in a triple.
            From that we calulated the shortest and longest distance
            and use that to scale the color of the intersection between
            that range.
                
        1.2.2: Normal-buffer
            For the normal-buffer we returned the normal of a hit point in the 
            trace method and normalized it so that clamp 
            could give a color for each value.

    1.3:    
        For all shapes that we added, we implemented the intersect 
        and normal calculation and added it to the makefile.
        
        Shapes that we added:
            -   Plane:
                -   We implemented an infinite plane, the parameters that we 
                    used are, a center (Point) and a orientation which is the 
                    same as the normal (Vector). Examples of this can be found 
                    in plane.yaml in the scenes folder.
            -   Triangle
                -   For the triangle we had to add a shape with 3 corners 
                    (Point) we also checked if the rectangle as behind the 
                    camera or if it was parallel to a ray, and if it was we 
                    returned that there was no hit. Examples of triangles can be 
                    found in triangle.yaml in the scenes folder. This scene 
                    contains two triangles forming a square. One of the 
                    triangles is matte, the other shiny.

            -   Bonus:
                -   Disk:
                    The disk is a non-infinite plane which is bound with a 
                    radius and makes a disk. An example of this can be found in 
                    disk.yaml
                -   Cylinder:  
                    The cylinder is defined as a finite tube along an axis, with 
                    a certain length. At both ends of the axis a disk is 
                    inserted. An example of a cylinder can be found in 
                    cylinder.yaml.
                -   Torus:
                    The torus is simply a donut shape. In order to solve the 
                    fourth degree equation it involved, we found a library that 
                    did this for us. Included in the algebra subdirectory, all 
                    credits to Don Cross. An example of this shape can be found 
                    in torus.yaml. All shapes in scene01 have also been replaced 
                    by tori. This scene is included as tori.yaml.

Comments:
-   All mentioned yaml files can found in the Scenes subdirectory. The corresponding 
    output images have been included in the Screenshots subdirectory. 

