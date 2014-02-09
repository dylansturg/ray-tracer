#ifndef SPHERE_H
#define SPHERE_H

#include "../Rays/Ray.h"
#include "../GenVector.h"
#include "Surface.h"

class Sphere : public Surface
{
public:
    Sphere()
    {

    }

    Sphere(Vector3 c, float r, int matIndex)
    {
        this->center = c;
        this->rad = r;
        this->materialId = matIndex;
    }

    virtual int getMaterialId() const
    {
        return this->materialId;
    }

    float getRadius() const
    {
        return this->rad;
    }
    virtual Vector3 getCenter() const
    {
        return this->center;
    }

    virtual Vector3 getMaxBoundaries() const
    {
        return Vector3(this->center[0] + this->rad, this->center[1] + this->rad, this->center[2] + this->rad);
    }
    virtual Vector3 getMinBoundaries() const
    {
        return Vector3(this->center[0] - this->rad, this->center[1] - this->rad, this->center[2] - this->rad);
    }

    void setRadius(float r)
    {
        this->rad = r;
    }
    void setCenter(Vector3 c)
    {
        this->center = c;
    }

    Vector3 getNormal(Vector3 point)
    {
        return (point - this->center).normalize();
    }

    virtual bool intersect(Ray &ray)
    {
        Vector3 direction = ray.getDirection();
        Vector3 origin = ray.getOrigin();
        Vector3 dirToSphere = origin - this->center;

        float a, b, c;

        a = direction.dot(direction);
        b = 2 * direction.dot(dirToSphere);
        c = dirToSphere.dot(dirToSphere) - (this->rad * this->rad);

        float discrim = calcDiscriminate(a, b, c);
        if (discrim < 0)
        {
            // negative discriminate means no real solutions
            return false;
        }

        float soln1, soln2;
        discrim = sqrt(discrim);
        soln1 = (-b + discrim) / (2 * a);
        soln2 = (-b - discrim) / (2 * a);
        bool ret = false;
        if (soln1 > 0)
        {
            ray.addHit(Hit(soln1, this->materialId, this->getNormal(ray.at(soln1))));
            ret = true;
        }
        if (soln2 > 0)
        {
            ray.addHit(Hit(soln2, this->materialId, this->getNormal(ray.at(soln2))));
            ret = true;
        }
        return ret;
    }

private:
    Vector3 center;
    float rad;
    int materialId;

    float calcDiscriminate(float a, float b, float c)
    {
        return b * b - 4 * a * c;
    }
};


#endif