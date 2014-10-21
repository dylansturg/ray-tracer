ray-tracer
==========

C++ Ray Tracer

The ray tracer uses OBJ files (with optional MTL data) to render images.  An OBJ file defines the geometry of an image using triangles.  The ray tracer works by reverse-simulating a camera: it renders an image based on tracing rays of light into the image outwards from the camera.  A traditional camera works by capturing light rays that enter the camera's lense (roughly).  Ray tracing allows rendering the image at any resolution, because any number of light rays may be simulated (based on the available time and computing resources).

I wrote almost all of the code for this project, except for:
	- anything in the objs/ directory: this is an OBJ file parser written by one of my professors
	- GenVector: this is an array based vector written by the same professor
	- simplePPM/ directory: this is a library for creating PPM images

I wrote every other class in the project.

The Makefile should build the project on UNIX based systems (I've tried it on Ubuntu and MacOSX).  There is a Visual Studio project in the WindowsRayTracer project that can be used.

The program accepts 3 arguments:
	./a.out [obj file name] [image resolution] [image file name]
	Example:
		./a.out objs/cornell_box.obj 512 cornell.ppm


