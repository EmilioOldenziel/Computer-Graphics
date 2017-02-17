Authors: Yannick Stoffers s2372061 & Emilio Oldenziel s2509679
Group: Tuesday 02

Raytracer 1
    1.1:
        1.1.1: Intersect Sphere
            For the intersection of a ray and sphere we had to use the abc formula.
            ...

        1.1.2: Normal Calculation
            ...

        1.1.3: Phong
            ...

    1.2:
        1.2.1: Z-buffer
            For the z-buffer we changed the trace class to only return the distance to the hit point in a triple.
            From that we calulated the shortest and longest distance and use that to scale the color of the intersection between that range.
                
        1.2.2: Normal-buffer
            ...

    1.3:    
        For all shapes that we added, we implemented the intersect and normal calculation and added it to the makefile.
        Shapes that we added:
            -   Plane:
                -   There are two versions that we added: an bound and infinite plane.
                    Parameters for a infinite plane in the YAML file are center (Point) and a orientation which is the same as the normal (Vector),
                    if you want the plane to be of a non-infinite size you can add w as width (float) and h as height.
                    Examples of this can be found in plane.yaml in the scenes folder.
            -   Triangle
                -   For the triangle we had to add a shape with 3 corners (Point) 
                    we also checked if the rectangle as behind the camera or if it was parallel to a ray, and if it was we returned that there was no hit.
                    Examples of triangles can be found in triangle.yaml in the scenes folder.

            -   Bonus:
                -   Cilinder?
                -   Thorus