Authors: Yannick Stoffers s2372061 & Emilio Oldenziel s2509679
Group: Tuesday 02

Raytracer 2:
    2.1: Optical laws
        -   We made some changes in scene for the optical laws
        2.1.1: Shadows
            -   Last time we already implemented of the light from the lightsource was blocked by an object.
                So we only had to apply this to the result and that got us the shadows.
        2.1.2: Light
            -   We already took this into account last time

        2.1.3: Refractions
            -   the maxrecursiondepth is read from the yaml file
            -   We calculated the refraction ray H and continiued the trace with H till the maxrecursiondepth is reached.

    2.2: Camera
        2.2.1: Supersampling
            -   first we read the Supersampling factor n from the yaml file
                this one will be used to take n^2 samples from the a grid setup.
                after this the samples are averaged and used to determine the color.

        2.2.2: Camera extention
            -   First we read the camera or eye from the yaml file if a camera is defined we use this to ...

Comments:
-   All mentioned yaml files can found in the Scenes subdirectory. The corresponding 
    output images have been included in the Screenshots subdirectory. 