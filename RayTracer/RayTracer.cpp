#include "GenVector.h"
#include "simplePPM/simplePPM.h"
#include "Buffer.h"
#include "Rays/Ray.h"
#include "Rays/RayGenerator.h"
#include "Rays/Hit.h"
#include "obj/objLoader.h"
#include "Camera.h"
#include "RayTracer.h"
#include <time.h>
#include <string.h>
#include "TimeWrapper.h"

#define DEFAULT_RESOLUTION 250

int main(int argc, char **argv)
{
    char *fileName;
    char *writeFileName;
    int res = DEFAULT_RESOLUTION;
    bool colorByNormal = false;
    if (argc < 2)
    {
        printf("Need an obj file to load from, using default.\n");
        fileName = "objs/day3.obj";
    }
    else
    {
        fileName = argv[1];
    }

    if (argc >= 3)
    {
        res = atoi(argv[2]);
        printf("using resolution %d\n", res);
    }

    if(argc >= 4){
        writeFileName = argv[3];
    } else {
        char writeName[10];
        strcpy(writeName, "test2.ppm");
        writeFileName = writeName;
    }

	long startTime = GetElapsedMS();

    RayTracer tracer = RayTracer(colorByNormal);

    tracer.processOBJ(fileName);

    tracer.setFrameBuffer(res, res);

    tracer.renderRayDirectionsToImage(writeFileName);

	printf("Time: %d(ms)\n", (GetElapsedMS() - startTime));

    return 0;
}

