#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Primitives/Sphere.h"
#include "Primitives/Triangle.h"
#include "Primitives/Material.h"
#include "Primitives/Surface.h"
#include "Primitives/Light.h"
#include "Rays/Ray.h"
#include "Rays/LightRay.h"
#include <vector>
#include "Primitives/BBVHTree.h"

#include <fstream>
#include <stdio.h>

using namespace std;

class Scene {
public:

	Vector3 cameraUp, cameraLookingAt;

	void parse(string binaryTrianglesFile, string binaryMaterialsFile, string binaryLightsFile, string binaryTreeFile, string binaryCameraFile)
	{
		fstream trianglesBinary(binaryTrianglesFile, ios::in | ios::binary);
		this->surfaces->empty();
		int index = 0;
		while (!trianglesBinary.eof()){
			this->surfaces->push_back(new Triangle());
			trianglesBinary.read((char*)((this->surfaces->at(index))), sizeof(Triangle));
			index++;
		}
		this->surfaces->pop_back(); // last one is extra, due to file reading junk
		trianglesBinary.close();

		fstream materialsBinary(binaryMaterialsFile, ios::in | ios::binary);
		this->materials.empty();
		index = 0;
		while (!materialsBinary.eof()){
			this->materials.push_back(Material());
			materialsBinary.read((char*)(&this->materials[index]), sizeof(Material));
			index++;
		}
		this->materials.pop_back();
		materialsBinary.close();

		fstream lightsBinary(binaryLightsFile, ios::in | ios::binary);
		this->lights.empty();
		index = 0;
		while (!lightsBinary.eof()){
			this->lights.push_back(Light());
			lightsBinary.read((char*)(&this->lights[index]), sizeof(Light));
			index++;
		}
		this->lights.pop_back();
		lightsBinary.close();


		fstream treeBinary(binaryTreeFile, ios::in | ios::binary);
		vector<Node>* treeNodes = new vector<Node>();
		index = 0;
		while (!treeBinary.eof()){
			treeNodes->push_back(Node());
			treeBinary.read((char*)(&(*treeNodes)[index]), sizeof(Node));
			index++;
		}
		treeNodes->pop_back();
		treeBinary.close();

		this->objectTree = BBVHTree(treeNodes);

		fstream cameraBinary(binaryCameraFile, ios::in | ios::binary);
		cameraBinary.read((char*)&(this->camera), sizeof(Camera));

		cameraBinary.read((char*)&(this->cameraLookingAt), sizeof(Vector3));
		cameraBinary.read((char*)&(this->cameraUp), sizeof(Vector3));

		cameraBinary.close();


	}

	Scene() {
		this->surfaces = new vector<Surface*>();
	}

	void parseObjFile(char *objFilename) {
		//struct timeval captureTime;
		objLoader *objData = new objLoader();

		//gettimeofday(&captureTime, NULL);
		//long objStart = captureTime.tv_sec * 1000000 + captureTime.tv_usec;
		objData->load(objFilename);
		//gettimeofday(&captureTime, NULL);
		//long objEnd = captureTime.tv_sec * 1000000 + captureTime.tv_usec;

		parseCamera(*objData);

		parseSpheres(*objData);
		parseTriangles(*objData);
		parseMaterials(*objData);
		parseLights(*objData);
		delete objData;
		printf("finished parsing the obj file: %d surfaces\n",
				this->surfaces->size());

		//gettimeofday(&captureTime, NULL);
		//long start = captureTime.tv_sec * 1000000 + captureTime.tv_usec;
		this->objectTree.initializeTree(this->surfaces);
		//gettimeofday(&captureTime, NULL);
		//long end = captureTime.tv_sec * 1000000 + captureTime.tv_usec;
		//printf("finished building scene tree of in %d usec, parsed obj in %d, total build time %d\n",
				//end - start, objEnd - objStart, end - objStart);

	}

	~Scene() {
		printf("destroying Scene\n");
	}

	bool intersect(Ray &ray) {
		return this->objectTree.intersect(ray);
	}

	vector<LightRay> *getRaysToLights(Vector3 point) {
		vector<LightRay> *lightRays = new vector<LightRay>();
		int lightCount = this->lights.size();
		for (int i = 0; i < lightCount; i++) {
			Vector3 dir = this->lights[i].getPosition() - point;
			lightRays->push_back(LightRay(point, dir, i));
		}
		return lightRays;
	}

	Camera getCamera() {
		return this->camera;
	}

	Material *getMaterial(int materialId) {
		if (materialId < 0 || materialId >= this->materials.size()) {
			return NULL;
		}
		return &this->materials[materialId];
	}

	Light *getLight(int lightId) {
		if (lightId < 0 || lightId >= this->lights.size()) {
			return NULL;
		}
		return &this->lights[lightId];
	}

	void SceneToBinFiles(string binaryTrianglesFile, string binaryMaterialsFile, string binaryLightsFile, string binaryTreeFile, string binaryCameraFile){
		fstream trianglesOut(binaryTrianglesFile, ios::binary | ios::out);

		for (int i = 0; i < this->surfaces->size(); i++){
			trianglesOut.write((char*)(this->surfaces->at(i)), sizeof(Triangle));
		}

		trianglesOut.close();

		fstream materialsOut(binaryMaterialsFile, ios::binary | ios::out);
		for (int i = 0; i < this->materials.size(); i++){
			materialsOut.write((char*)(&this->materials[i]), sizeof(Material));
		}
		materialsOut.close();

		fstream lightsOut(binaryLightsFile, ios::binary | ios::out);
		for (int i = 0; i < this->lights.size(); i++){
			lightsOut.write((char*)(&this->lights[i]), sizeof(Light));
		}
		lightsOut.close();

		fstream treeOut(binaryTreeFile, ios::binary | ios::out);
		vector<Node>* tree = this->objectTree.getNodeList();
		for (int i = 0; i < tree->size(); i++){
			treeOut.write((char*)(&(*tree)[i]), sizeof(Node));
		}
		treeOut.close();

		fstream cameraOut(binaryCameraFile, ios::binary | ios::out);
		cameraOut.write((char*)&(this->camera), sizeof(Camera));
		cameraOut.write((char*)&(this->cameraLookingAt), sizeof(Vector3));
		cameraOut.write((char*)&(this->cameraUp), sizeof(Vector3));
		cameraOut.close();

		return;
	}

private:
	Camera camera;
	vector<Surface *>* surfaces;
	vector<Material> materials;
	vector<Light> lights;
	BBVHTree objectTree;

	void parseCamera(objLoader &objData) {
		if (objData.camera == NULL) {
			printf("obj file needs a camera.\n");
			exit(1);
		}

		Vector3 cameraPosition = Vector3(
				objData.vertexList[objData.camera->camera_pos_index]->e[0],
				objData.vertexList[objData.camera->camera_pos_index]->e[1],
				objData.vertexList[objData.camera->camera_pos_index]->e[2]);

		Vector3 cameraLookingAt =
				Vector3(
						objData.vertexList[objData.camera->camera_look_point_index]->e[0],
						objData.vertexList[objData.camera->camera_look_point_index]->e[1],
						objData.vertexList[objData.camera->camera_look_point_index]->e[2]);

		Vector3 cameraUpNormal = Vector3(
				objData.normalList[objData.camera->camera_up_norm_index]->e[0],
				objData.normalList[objData.camera->camera_up_norm_index]->e[1],
				objData.normalList[objData.camera->camera_up_norm_index]->e[2]);

		this->camera = Camera(cameraPosition, cameraLookingAt, cameraUpNormal,
				90.0f);
	}

	void parseMaterials(objLoader &objData) {
		for (int i = 0; i < objData.materialCount; i++) {
			this->materials.push_back(Material(objData.materialList[i]));
		}
	}

	void parseSpheres(objLoader &objData) {
		for (int i = 0; i < objData.sphereCount; i++) {
			obj_sphere *o = objData.sphereList[i];
			Vector3 center = Vector3(objData.vertexList[o->pos_index]->e[0],
					objData.vertexList[o->pos_index]->e[1],
					objData.vertexList[o->pos_index]->e[2]);
			Vector3 upNorm = Vector3(
					objData.normalList[o->up_normal_index]->e[0],
					objData.normalList[o->up_normal_index]->e[1],
					objData.normalList[o->up_normal_index]->e[2]);
			float rad = upNorm.length();
			this->surfaces->push_back(
					new Sphere(center, rad, o->material_index));
		}
	}

	void parseTriangles(objLoader &objData) {
		for (int i = 0; i < objData.faceCount; i++) {
			obj_face *o = objData.faceList[i];
			Vector3 vertices[3];
			for (int j = 0; j < 3; j++) {
				vertices[j] = Vector3(
						objData.vertexList[o->vertex_index[j]]->e[0],
						objData.vertexList[o->vertex_index[j]]->e[1],
						objData.vertexList[o->vertex_index[j]]->e[2]);
			}
			this->surfaces->push_back(
					new Triangle(vertices[0], vertices[1], vertices[2],
							o->material_index));
		}
	}

	void parseLights(objLoader &objData) {
		for (int i = 0; i < objData.lightPointCount; i++) {
			obj_light_point *pl = objData.lightPointList[i];
			double *pos = objData.vertexList[pl->pos_index]->e;
			obj_material *mat = objData.materialList[pl->material_index];
			this->lights.push_back(Light(mat, pos));
		}
	}

};

#endif
