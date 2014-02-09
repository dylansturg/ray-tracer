#ifndef SURFACE_H
#define SURFACE_H

#include "../Rays/Ray.h"
#include "../GenVector.h"

class Surface
{
public:

    virtual ~Surface()
    {
    }

    virtual bool intersect(Ray &ray) = 0;

    virtual int getMaterialId() const
    {
        return -1;
    }

    virtual Vector3 getCenter() const = 0;

    virtual Vector3 getMaxBoundaries() const = 0;
    virtual Vector3 getMinBoundaries() const = 0;

    virtual int height(){ return 0; }

private:

};



#endif
