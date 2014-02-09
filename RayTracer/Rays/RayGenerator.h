#pragma once

#include "../GenVector.h"
#include "../Camera.h"
#include "Ray.h"
#include <vector>

using namespace std;

class RayGenerator
{
public:
    RayGenerator(Camera c, unsigned int w, unsigned int h)
    {
        this->width = (float)w;
        this->height = (float)h;
        this->imageDistance = (float)(h >> 1);
        this->camera = c;

        // was originally pre-computing all of the rays, but I'm going to try a lazy strategy
        //generateRays();
    }

    ~RayGenerator()
    {

    }

    Ray getRayAt(unsigned int x, unsigned int y)
    {
        Vector3 rayOrigin = this->camera.getPosition();
        // float imageDist = (float)(this->height >> 1); //calculateDistToImagePlane(this->camera.getFOV());

        float l, b, w, h;

        w = (float)this->width;
        h = (float)this->height;

        l = -((float)(this->width >> 1));
        b = -((float)(this->height >> 1));

        float uPos = l + (w * ((float)x + 0.5)) / w;
        float vPos = b + (h * ((float)y + 0.5)) / h;

        Vector3 rayDir = (-this->imageDistance * this->camera.getW()) + (uPos * this->camera.getU()) + (vPos * this->camera.getV());
        return Ray(rayOrigin, rayDir);
    }

private:
    vector<Ray> rays;
    unsigned int width;
    unsigned int height;
    Camera camera;
    float imageDistance;

    void generateRays()
    {
        rays.resize(this->width * this->height);

        Vector3 rayOrigin = this->camera.getPosition();
        float imageDist = calculateDistToImagePlane(this->camera.getFOV());

        for (int y = 0; y < this->height; y++)
        {
            for (int x = 0; x < this->width; x++)
            {
                float uPos = ((float) this->width * ((float)x + 0.5)) / (float)this->width;
                float vPos = ((float) this->height * ((float)y + 0.5)) / (float)this->height;
                this->rays[x + y * this->width].setOrigin(rayOrigin);
                this->rays[x + y * this->width].setDirection((-imageDist * this->camera.getW()) +
                        (uPos * this->camera.getU()) + (vPos * this->camera.getV()));
            }
        }
    }

    float calculateDistToImagePlane(float fov)
    {
        // for now, hard code to 90 degree fov... maybe fix later
        return ((float)this->height) / 2;
    }


};