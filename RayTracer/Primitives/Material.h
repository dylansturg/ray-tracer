#ifndef MATERIAL_H
#define MATERIAL_H

#include "../obj/objLoader.h"

class Material
{
public:
    Material()
    {

    }

    Material(obj_material *mat)
    {
        this->ambientColor = objVecToColor(mat->amb);
        this->diffuseColor = objVecToColor(mat->diff);
        this->specularColor = objVecToColor(mat->spec);

        this->ambient = objVecToVec(mat->amb);
        this->diffuse = objVecToVec(mat->diff);
        this->specular = objVecToVec(mat->spec);

        this->reflectance = (float) mat->reflect;
        this->refraction = (float) mat->refract;
        this->transparency = (float) mat->trans;
        this->shiny = (float) mat->shiny;
        this->glossy = (float) mat->glossy;
    }

    ~Material()
    {

    }

    Color getAmbientColor()
    {
        return this->ambientColor;
    }

    Vector3 getAmbient(){
        return this->ambient;
    }

    Vector3 getDiffuse(){
        return this->diffuse;
    }

    Vector3 getSpecular(){
        return this->specular;
    }

    float getReflectance(){
        return this->reflectance;
    }

    float getShiny(){
        return this->shiny;
    }

private:
    Color ambientColor, diffuseColor, specularColor;
    Vector3 ambient, diffuse, specular;
    float reflectance, refraction, transparency, shiny, glossy;

    Color objVecToColor(double vec[])
    {
        return (Color(vec[0]* 255.0f, vec[1]* 255.0f, vec[2]* 255.0f) );
    }

    Vector3 objVecToVec(double vec[]){
        return Vector3(vec[0], vec[1], vec[2]);
    }

};

#endif