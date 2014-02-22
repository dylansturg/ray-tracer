#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "Rays/Ray.h"
#include "Primitives/Sphere.h"
#include <cmath>
#include "simplePPM/simplePPM.h"
#include "obj/objLoader.h"
#include <vector>
#include "Scene.h"

using namespace std;

class RayTracer {
public:

	RayTracer() {
		this->colorByNormal = false;
	}

	RayTracer(bool normColor) {
		this->colorByNormal = false;

		
	}

	~RayTracer() {
	}

	void drawFrameBuffer(char *fileName) {
		simplePPM_write_ppm(fileName, this->frameBuffer.getWidth(),
				this->frameBuffer.getHeight(),
				(unsigned char *) &this->frameBuffer.at(0, 0));
	}

	void setFrameBuffer(unsigned int w, unsigned int h) {
		this->frameBuffer = Buffer(w, h);
	}

	void processOBJ(char *fileName) {
		this->scene.parseObjFile(fileName);
	}

	void renderRayDirectionsToImage(char *fileName) {
		RayGenerator rg = RayGenerator(this->scene.getCamera(),
				this->frameBuffer.getWidth(), this->frameBuffer.getHeight());
		drawRaysToBuffer(this->frameBuffer, rg);
		drawFrameBuffer(fileName);
	}

private:
	Buffer frameBuffer;
	Scene scene;
	bool colorByNormal;

	void drawRaysToBuffer(Buffer &b, RayGenerator &rg) {
		float maximumValue = 0.0f;
		vector<Vector3> sceneColors;
		unsigned int h = b.getHeight();
		unsigned int w = b.getWidth();
		sceneColors.resize(h * w);
#pragma omp parallel for
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				Ray r = rg.getRayAt(x, y);
				Vector3 c = traceRay(r);
				sceneColors[x + (y * h)] = c;
			}

		}

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				Vector3 c = sceneColors[x + (y * h)];
				maximumValue = max(maximumValue, max(c[0], max(c[1], c[2])));
			}

		}
#pragma omp parallel for
		for (int y = 1; y <= h; y++) {
			for (int x = 0; x < w; x++) {

				Vector3 sceneCol = sceneColors[x + (y - 1) * h];
				sceneCol /= maximumValue;
				sceneCol *= 255.0f;
				Color &buffColor = this->frameBuffer.at(x, h - y);
				buffColor = sceneCol;
			}

		}
	}

	Vector3 traceRay(Ray &ray) {
		return traceRay(ray, 25);
	}

	Vector3 traceRay(Ray &ray, int recursionDepth) {
		if (recursionDepth < 0) {
			return Vector3(0, 0, 0);
		}
		this->scene.intersect(ray);
		Hit hit = ray.getFirstHit();
		float t = hit.getParam();

		Vector3 c = Vector3(0, 0, 0);
		if (t > 0) {
			// ray got a hit!

			if (colorByNormal) {
				Vector3 normal = hit.getHitNormal();
				normal *= 255.0f;
				c = Vector3(abs(normal[0]), abs(normal[1]), abs(normal[2]));
			} else {
				c = shadePixel(hit, ray, recursionDepth);
			}
		}
		return c;

	}

	Vector3 shadePixel(Hit &hit, Ray &ray, int recursionDepth) {
		Material *hitMaterial = this->scene.getMaterial(hit.getHitMaterialId());

#ifdef NULL_CHECKS
		if (hitMaterial == NULL) {
			printf("hit something with no material\n");
			return Vector3(250, 250, 250);
		}
#endif
		Vector3 normal = hit.getHitNormal();
		Vector3 adjustedHitPos = ray.at(hit.getParam());
		Vector3 view = this->scene.getCamera().getPosition() - adjustedHitPos;
		view.normalize();

		float epsilon = 0.0001;
		adjustedHitPos += epsilon * normal;
		vector<LightRay> *lightRays = this->scene.getRaysToLights(
				adjustedHitPos);
		int lrCount = lightRays->size();
		Vector3 pixelColor = Vector3(0, 0, 0);
		float specularPow = hitMaterial->getShiny();
		for (int i = 0; i < lrCount; i++) {
			LightRay r = (*lightRays)[i];
			Light *hitLight = this->scene.getLight(r.getLightId());

			this->scene.intersect(r);
			float hitTime = r.getFirstHit().getParam();
			bool inShadow = true;
			if (hitTime < 0) {
				inShadow = false;
			} else {
				float distanceToHit =
						(r.at(hitTime) - r.getOrigin()).squaredLength();
				inShadow = distanceToHit < r.getSquaredLightDistance();
			}

			pixelColor += hitLight->getAmbient() * hitMaterial->getAmbient();
			if (!inShadow) {
				// clear ray to light... light it up!
				Vector3 lightReflect = (2 * (r.getDirection().dot(normal))
						* normal - r.getDirection()).normalize();
				Vector3 diffColor = hitLight->getDiffuse()
						* hitMaterial->getDiffuse()
						* clamp(normal.dot(r.getDirection()), 0.0f);
				Vector3 specColor = hitLight->getSpecular()
						* hitMaterial->getSpecular()
						* pow(clamp(view.dot(lightReflect), 0.0f),
								specularPow);
				pixelColor += diffColor + specColor;
			}
		}

		// add a reflection
		if (hitMaterial->getReflectance() > 0) {
			Vector3 reflectionDir = 2 * (view.dot(normal)) * normal - view;
			Ray reflectionRay = Ray(adjustedHitPos, reflectionDir);
			Vector3 reflectionColor = hitMaterial->getReflectance()
					* traceRay(reflectionRay, recursionDepth - 1);
			pixelColor += reflectionColor;
		}
		delete lightRays;
		return pixelColor;
	}

	float clamp(float val, float min) {
		if (val < min) {
			return min;
		}
		return val;
	}

};

#endif
