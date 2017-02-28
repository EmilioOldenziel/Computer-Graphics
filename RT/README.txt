Authors: Yannick Stoffers s2372061 & Emilio Oldenziel s2509679
Group: Tuesday 02

Raytracer 1
    1.1:
        1.1.1: Intersect Sphere
            We implemented sphere intersection in the sphere class.
            For the intersection of a ray and sphere we had to use the
            abc formula. 

        1.1.2: Normal Calculation
            The normal is the vector which is orthogonal to the surface of the hit point.

        1.1.3: Phong
            We implemented the Phong illumination in the scene.
            We added ambient, diffuse and specular lighting from each light source.

    1.2:
        First we introduced 3 rendermodes in the yaml file: 
        zbuffer,normal and phong. If no rendermode is give phong wil be used as default.

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
                -   We implemented an infinite plane, the parameters that we used are, 
                    a center (Point) and a orientation which is the same as the normal (Vector).
                    Examples of this can be found in plane.yaml in the scenes folder.
            -   Triangle
                -   For the triangle we had to add a shape with 3 corners (Point) 
                    we also checked if the rectangle as behind the camera or if it was 
                    parallel to a ray, and if it was we returned that there was no hit.
                    Examples of triangles can be found in triangle.yaml in the scenes 
                    folder. This scene contains two triangles forming a square. One of 
                    the triangles is matte, the other shiny.

            -   Bonus:
                -   Disk:
                        The disk is a non-infinite plane which is bound with a radius
                        and makes a disk. An example of this can be found in disk.yaml
                -   Cylinder:  
                        The cylinder is defined as a finite tube along an axis, with a 
                        certain length. At both ends of the axis a disk is inserted. An 
                        example of a cylinder can be found in cylinder.yaml.
                -   Torus:
                        The torus is simply a donut shape. In order to solve the fourth 
                        degree equation it involved, we found a library that did this 
                        for us. Included in the algebra subdirectory, all credits to 
                        Don Cross. An example of this shape can be found in torus.yaml. 
                        All shapes in scene01 have also been replaced by tori. This scene 
                        is included as tori.yaml.

Comments:
-   All mentioned yaml files can found in the Scenes subdirectory. The corresponding 
    output images have been included in the Screenshots subdirectory. 
