#pragma once

#include "Vec3.hpp"
#include "Triangle.hpp"
#include "Mesh.hpp"

namespace graphics {

	const char shades[] = { '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@' };
	const int numberOfShades = 12;
	enum drawTriangleMode;
	enum drawMeshMode;

	Vec3 rotatePoint(const Vec3& point, const Vec3& rotation); //rotate point in 3D space
	Triangle rotateTriangle(const Triangle& triangle, const Vec3& rotation); //rotate triangle in 3D space
	Mesh rotateMesh(const Mesh& mesh, const Vec3& rotation); //rotate mesh in 3D space
	Vec3 translatePoint(const Vec3& point, const Vec3& offset); //move point in 3D space
	Triangle translateTriangle(const Triangle& triangle, const Vec3& offset); //move triangle in 3D space
	Mesh translateMesh(const Mesh& mesh, const Vec3& offset); //move mesh in 3D space
	Vec3 projectPoint(const Vec3& point, const int& screenWidth, const int& screenHeight, const int& FOV); //project point to console screen space
	Triangle projectTriangle(const Triangle& triangle, const int& screenWidth, const int& screenHeight, const int& FOV); //project triangle to console screen space
	Mesh projectMesh(const Mesh& mesh, const int& screenWidth, const int& screenHeight, const int& FOV); //project mesh to console screen space

	char shadeTriangle(const Triangle& triangle, const Vec3& lightDirection);

	Vec3 toConsoleCoordinates(const Vec3& coord, const int& screenWidth, const int& screenHeight); //translate from [-1.0, 1.0] coordinates to [0, screenWidth] and [0, screenHeight] coordinates

	void rasterisePoint(const Vec3& point, const char& fill, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer); //insert a point into buffer
	void rasteriseLine(const Vec3& lineStart, const Vec3& lineEnd, const char& fill, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer); //insert a line into buffer
	void rasteriseTriangle(const Triangle& triangle, const drawTriangleMode& mode, const char& fill, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer); //insert a triangle into buffer


	void drawMesh(const Mesh& mesh, const drawMeshMode& mode, const Vec3& lightDirection, const int& screenWidth, const int& screenHeight, const int& FOV, std::string& frameBuffer, double* zBuffer); //insert mesh into buffer

	void drawFrame(std::string& buffer, const int& screenWidth, const int& screenHeight);

	enum drawTriangleMode {
		TRIANGLE_FULL,
		TRIANGLE_SHADED,
		TRIANGLE_EDGES,
		TRIANGLE_VERTICES
	};

	enum drawMeshMode {
		MESH_FULL,
		MESH_SHADED,
		MESH_EDGES,
		MESH_VERTICES
	};
}