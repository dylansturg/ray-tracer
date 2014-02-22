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

#define DEFAULT_RESOLUTION 2048
#define FRAME_COUNT 25

int main(int argc, char **argv)
{
    char *fileName;
    char *writeFileName;
    int res = DEFAULT_RESOLUTION;
    bool colorByNormal = false;
    if (argc < 2)
    {
        printf("Need an obj file to load from, using default.\n");
        fileName = "dragon.obj";
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

	int runTimes[FRAME_COUNT];

    RayTracer tracer = RayTracer(colorByNormal);

    tracer.processOBJ("dragon.obj");
	int resolutions[] = { 256, 512, 1024, 2048 };
	for (int frame = 0; frame < 4; frame++){

		tracer.setFrameBuffer(resolutions[frame], resolutions[frame]);
		for (int i = 0; i < FRAME_COUNT; i++){
			long startTime = GetElapsedMS();

			tracer.renderRayDirectionsToImage(writeFileName);

			runTimes[i] = GetElapsedMS() - startTime;
		}

		int sum = 0;
		for (int i = 0; i < FRAME_COUNT; i++){
			printf("Run time (%d): %d\n", i, runTimes[i]);
			sum += runTimes[i];
		}

		printf("Average: %d\n", sum / FRAME_COUNT);
	}

    return 0;
}

