#ifndef _LIGHT_H
#define _LIGHT_H

#include "../GenVector.h"
#include "../obj/objLoader.h"

class Light{
public:
	Light(){

	}

	Light(obj_material* lightMat, double pos[]){
		this->IAmbient = arrayToVector(lightMat->amb);
		this->IDiffuse = arrayToVector(lightMat->diff);
		this->ISpecular = arrayToVector(lightMat->spec);
		this->Position = arrayToVector(pos);
	}

	Vector3 getAmbient() const{
		return this->IAmbient;
	}

	Vector3 getDiffuse() const{
		return this->IDiffuse;
	}

	Vector3 getSpecular() const{
		return this->ISpecular;
	}

	Vector3 getPosition() const {
		return this->Position;
	}

private:
	Vector3 IAmbient, IDiffuse, ISpecular, Position;

	Vector3 arrayToVector(double e[]){
		return Vector3(e[0], e[1], e[2]);
	}

};


#endif