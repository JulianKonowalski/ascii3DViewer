#pragma once

#include "Vec3.h"
#include "Triangle.h"

namespace graphics {

	enum drawTriangleMode;

	Vec3 rotate(const Vec3& point, const Vec3& rotation); //rotate point in 3D space
	Vec3 translate(const Vec3& point, const Vec3& offset); //move point in 3D space
	Vec3 projectPoint(Vec3 point, const int& screenWidth, const int& screenHeight, const int& FOV); //project point to console screen space
	Triangle projectTriangle(Triangle triangle, const int& screenWidth, const int& screenHeight, const int& FOV); //project triangle to console screen space

	Vec3 toConsoleCoordinates(const Vec3& coord, const int& screenWidth, const int& screenHeight); //translate from [-1.0, 1.0] coordinates to [0, screenWidth] and [0, screenHeight] coordinates

	void drawPoint(const Vec3& point, int& screenWidth, char* buffer);
	void drawLine(const Vec3& lineStart, const Vec3& lineEnd, const int& screenWidth, char* buffer);
	void drawTriangle(const Triangle& triangle, drawTriangleMode mode, const int& screenWidth, char* buffer);

	enum drawTriangleMode {
		TRIANGLE_FULL,
		TRIANGLE_INSIDE,
		TRIANGLE_OUTLINE,
		TRIANGLE_VERTICES
	};
}