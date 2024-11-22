#pragma once

#include "Vec3.h"

namespace graphics {

	Vec3 rotate(const Vec3& point, const Vec3& rotation); //rotate point in 3D space
	Vec3 translate(const Vec3& point, const Vec3& offset); //move point in 3D space
	Vec3 project(const Vec3& vector, const int FOV); //project point to screen space

	Vec3 toConsoleCoordinates(const Vec3& coord, const int width, const int height); //translate from [-1.0, 1.0] coordinates to [0, screenWidth] and [0, screenHeight] coordinates

	void drawPoint(const Vec3& point, int width, char* buffer);
	void drawLine(const Vec3& lineStart, const Vec3& lineEnd, int width, char* buffer);
	void fillTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3, int width, char* buffer);
}