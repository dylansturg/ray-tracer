#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../Rays/Ray.h"
#include "../GenVector.h"
#include "Surface.h"

class Triangle: public Surface {

public:
	Triangle() {

	}

	Triangle(Vector3 a, Vector3 b, Vector3 c, int matIndex) {
		// Receive the 3 coordinates of a triangle.
		this->vertices[0] = a;
		this->vertices[1] = b;
		this->vertices[2] = c;
		this->materialId = matIndex;

		this->normal = (b - a).cross((c - a));
		this->normal.normalize();

		this->setCenter();
		this->setBoundaries();
	}

	virtual Vector3 getCenter() const {
		return this->center;
	}

	Vector3 getNormal() {
		return this->normal;
	}

	virtual int getMaterialId() const {
		return this->materialId;
	}

	virtual Vector3 getMaxBoundaries() const {
		return this->maxBoundaries;
	}
	virtual Vector3 getMinBoundaries() const {
		return this->minBoundaries;
	}

	virtual bool intersect(Ray &ray) {
		float a, b, c, d, e, f, g, h, i, j, k, l;
		float ei_minus_hf, gf_minus_di, dh_minus_eg;
		float ak_minus_jb, jc_minus_al, bl_minus_kc;
		float beta, gamma, M;

		Vector3 A, B, C, D, E;
		A = this->vertices[0];
		B = this->vertices[1];
		C = this->vertices[2];
		D = ray.getDirection();
		E = ray.getOrigin();

		a = A[0] - B[0];
		b = A[1] - B[1];
		c = A[2] - B[2];

		d = A[0] - C[0];
		e = A[1] - C[1];
		f = A[2] - C[2];

		g = D[0];
		h = D[1];
		i = D[2];

		j = A[0] - E[0];
		k = A[1] - E[1];
		l = A[2] - E[2];

		ei_minus_hf = e * i - h * f;
		gf_minus_di = g * f - d * i;
		dh_minus_eg = d * h - e * g;

		// start calculating interesting values
		M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;

		if (M == 0) {
			return false;
		}

		beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / M;

		if (beta < 0 || beta > 1) {
			// half plane test failed
			return false;
		}

		ak_minus_jb = a * k - j * b;
		jc_minus_al = j * c - a * l;
		bl_minus_kc = b * l - k * c;

		gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / M;

		if (gamma < 0 || gamma > 1) {
			// half plane test failed
			return false;
		}

		// got a hit.. calculate the parameter

		float t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / M;
		if (t > 0 && beta + gamma <= 1) {
			ray.addHit(Hit(t, this->materialId, this->getNormal()));
			return true;
		} else {
			return false;
		}

	}

private:
	Vector3 vertices[3];
	Vector3 normal;
	Vector3 maxBoundaries, minBoundaries;
	Vector3 center;
	int materialId;

	void setCenter() {
		Vector3 center = Vector3(0, 0, 0);
		for (int i = 0; i < 3; i++) {
			center[0] += this->vertices[i][0];
			center[1] += this->vertices[i][1];
			center[2] += this->vertices[i][2];
		}
		this->center = (center / 3);
	}
	void setBoundaries() {
		float minX =
				this->vertices[0][0] < this->vertices[1][0] ?
						(this->vertices[0][0] < this->vertices[2][0] ?
								this->vertices[0][0] : this->vertices[2][0]) :
						(this->vertices[1][0] < this->vertices[2][0] ?
								this->vertices[1][0] : this->vertices[2][0]);
		float minY =
				this->vertices[0][1] < this->vertices[1][1] ?
						(this->vertices[0][1] < this->vertices[2][1] ?
								this->vertices[0][1] : this->vertices[2][1]) :
						(this->vertices[1][1] < this->vertices[2][1] ?
								this->vertices[1][1] : this->vertices[2][1]);
		float minZ =
				this->vertices[0][2] < this->vertices[1][2] ?
						(this->vertices[0][2] < this->vertices[2][2] ?
								this->vertices[0][2] : this->vertices[2][2]) :
						(this->vertices[1][2] < this->vertices[2][2] ?
								this->vertices[1][2] : this->vertices[2][2]);
		this->minBoundaries = Vector3(minX, minY, minZ);

		float maxX =
				this->vertices[0][0] > this->vertices[1][0] ?
						(this->vertices[0][0] > this->vertices[2][0] ?
								this->vertices[0][0] : this->vertices[2][0]) :
						(this->vertices[1][0] > this->vertices[2][0] ?
								this->vertices[1][0] : this->vertices[2][0]);
		float maxY =
				this->vertices[0][1] > this->vertices[1][1] ?
						(this->vertices[0][1] > this->vertices[2][1] ?
								this->vertices[0][1] : this->vertices[2][1]) :
						(this->vertices[1][1] > this->vertices[2][1] ?
								this->vertices[1][1] : this->vertices[2][1]);

		float maxZ =
				this->vertices[0][2] > this->vertices[1][2] ?
						(this->vertices[0][2] > this->vertices[2][2] ?
								this->vertices[0][2] : this->vertices[2][2]) :
						(this->vertices[1][2] > this->vertices[2][2] ?
								this->vertices[1][2] : this->vertices[2][2]);
		this->maxBoundaries = Vector3(maxX, maxY, maxZ);

	}

};

#endif
