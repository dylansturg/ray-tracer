#pragma once

#include "GenVector.h"

class Camera
{
public:
    Camera(){
        
    }

    Camera(Vector3 pos, Vector3 lookingAt, Vector3 up, float fov)
    {
        this->e = pos;
        setFOV(fov);
        setBasis(pos, lookingAt, up);
    }

    ~Camera()
    {
    }

    void setBasis(Vector3 pos, Vector3 lookingAt, Vector3 up)
    {
        calculateBasisVectors(pos, lookingAt, up);
    }

    void setFOV(float fov)
    {
        this->fieldOfView = fov;
    }

    float getFOV() const
    {
        return this->fieldOfView;
    }

    Vector3 getU()
    {
        return this->u;
    }
    Vector3 getV()
    {
        return this->v;
    }
    Vector3 getW()
    {
        return this->w;
    }
    Vector3 getPosition()
    {
        return this->e;
    }

private:
    Vector3 u, v, w, e;
    float fieldOfView;

    void calculateBasisVectors(Vector3 pos, Vector3 lookingAt, Vector3 up)
    {
        this->w = pos - lookingAt; //negative looking at, same as -(l - p)
        this->u = up.cross(this->w);
        this->v = this->w.cross(this->u);

        this->w.normalize();
        this->u.normalize();
        this->v.normalize();

    }
};