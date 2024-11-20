#pragma once

#include "Vec2.h"

class Triangle2D {
public:
	Triangle2D(const Vec2* vertices) {
		for (int i = 0; i < 3; ++i) { mVertices[i] = vertices[i]; }
	}

	Vec2 p1(void) const { return mVertices[0]; }
	Vec2 p2(void) const { return mVertices[1]; }
	Vec2 p3(void) const { return mVertices[2]; }

	void setP1(Vec2 p) { mVertices[0] = p; }
	void setP2(Vec2 p) { mVertices[1] = p; }
	void setP3(Vec2 p) { mVertices[2] = p; }

private:
	Vec2 mVertices[3];
};