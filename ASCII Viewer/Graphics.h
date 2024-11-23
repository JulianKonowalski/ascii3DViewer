#pragma once

#include "Vec3.h"
#include "Triangle.h"

namespace graphics {

	enum drawTriangleMode;

	std::string getBuffer(const int& screenWidth, const int& screenHeight);
	void clearBuffer(std::string& buffer, const int& screenWidth, const int& screenHeight);

	Vec3 rotate(const Vec3& point, const Vec3& rotation); //rotate point in 3D space
	Vec3 translate(const Vec3& point, const Vec3& offset); //move point in 3D space
	Vec3 projectPoint(Vec3 point, const int& screenWidth, const int& screenHeight, const int& FOV); //project point to console screen space
	Triangle projectTriangle(Triangle triangle, const int& screenWidth, const int& screenHeight, const int& FOV); //project triangle to console screen space

	Vec3 toConsoleCoordinates(const Vec3& coord, const int& screenWidth, const int& screenHeight); //translate from [-1.0, 1.0] coordinates to [0, screenWidth] and [0, screenHeight] coordinates

	void rasterisePoint(const Vec3& point, const int& screenWidth, std::string& buffer); //insert a point into buffer
	void rasteriseLine(const Vec3& lineStart, const Vec3& lineEnd, const int& screenWidth, std::string& buffer); //insert a line into buffer
	void rasteriseTriangle(const Triangle& triangle, const drawTriangleMode& mode, const char& shade, const int& screenWidth, std::string& buffer); //insert a triangle into buffer

	void drawFrame(std::string& buffer, const int& screenWidth, const int& screenHeight);

	enum drawTriangleMode {
		TRIANGLE_FULL,
		TRIANGLE_INSIDE,
		TRIANGLE_OUTLINE,
		TRIANGLE_VERTICES
	};
}