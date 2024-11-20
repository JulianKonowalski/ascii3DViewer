#pragma once

#include "Vec3.h"

class Triangle3D {
public:
	Triangle3D(const Vec3* vertices) {
		for (int i = 0; i < 3; ++i) { mVertices[i] = vertices[i]; }
	}

	Vec3 p1(void) const { return mVertices[0]; }
	Vec3 p2(void) const { return mVertices[1]; }
	Vec3 p3(void) const { return mVertices[2]; }

	void setP1(Vec3 point) { mVertices[0] = point; }
	void setP2(Vec3 point) { mVertices[1] = point; }
	void setP3(Vec3 point) { mVertices[2] = point; }

private:
	Vec3 mVertices[3];
};