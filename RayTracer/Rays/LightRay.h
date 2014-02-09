#ifndef _LIGHT_RAY_H
#define _LIGHT_RAY_H

class LightRay : public Ray
{
public:
    LightRay(Vector3 origin, Vector3 dir, int lid)
        : Ray(origin, dir)
    {
        this->lightId = lid;
        this->squaredLightDistance = dir.squaredLength();
    }

    int getLightId()
    {
        return this->lightId;
    }

    void setLightId(int l)
    {
        this->lightId = l;
    }

    float getSquaredLightDistance(){
        return this->squaredLightDistance;
    }

private:
    int lightId;
    float squaredLightDistance;

};


#endif
