#pragma once

#include "Vec3.hpp"

class Triangle {
public:
	Triangle(const Vec3* vertices) {
		for (int i = 0; i < 3; ++i) { mVertices[i] = vertices[i]; }
	}

	Triangle(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
		mVertices[0] = v1;
		mVertices[1] = v2;
		mVertices[2] = v3;
	}

	Triangle() {
		mVertices[0] = Vec3();
		mVertices[1] = Vec3();
		mVertices[2] = Vec3();
	}

	const Vec3* getVertices(void) const { return mVertices; }
	Vec3 p1(void) const { return mVertices[0]; }
	Vec3 p2(void) const { return mVertices[1]; }
	Vec3 p3(void) const { return mVertices[2]; }

	void setP1(const Vec3& point) { mVertices[0] = point; }
	void setP2(const Vec3& point) { mVertices[1] = point; }
	void setP3(const Vec3& point) { mVertices[2] = point; }

private:
	Vec3 mVertices[3];
};