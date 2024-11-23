#include "Graphics.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <thread>

const int WIDTH = 150, HEIGHT = 75;
const int FOV = 45;

Mesh createMesh(void) {
	std::vector<Vec3> vertices = {
		Vec3(0.0, 1.0, 0.0),
		Vec3(0.0, 0.0, 1.0),
		Vec3(1.0, 0.0, 0.0),
		Vec3(-1.0, 0.0, 0.0)
	};
	std::vector<Vec3> indices = {
		Vec3(1, 2, 3),
		Vec3(1, 3, 4),
		Vec3(1, 4, 2),
		Vec3(2, 3, 4)
	};
	return Mesh(vertices, indices);
}

using namespace graphics;

int main(void) {

	std::string buffer = getBuffer(WIDTH, HEIGHT);

	Mesh mesh = createMesh();

	Vec3 rotationVector(0.0, 10.0, 10.0);
	Vec3 translationVector(0.0, 0.0, 5.0);

	while (true) {

		mesh = rotateMesh(mesh, rotationVector);

		Mesh translatedMesh = translateMesh(mesh, translationVector);

		rasteriseMesh(translatedMesh, MESH_FULL, Vec3(0.0, 0.0, 1.0), WIDTH, HEIGHT, FOV, buffer);

		drawFrame(buffer, WIDTH, HEIGHT);
		clearBuffer(buffer, WIDTH, HEIGHT);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}