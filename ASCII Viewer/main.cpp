#include <algorithm>
#include <iostream>
#include <thread>

#include "Graphics.hpp"
#include "Mesh.hpp"
#include "ObjFileLoader.hpp"

const int WIDTH = 400, HEIGHT = 200;
const int SCREEN_SIZE = WIDTH * HEIGHT;
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
		Vec3(4, 3, 2)
	};
	return Mesh(vertices, indices);
}

using namespace graphics;

int main(void) {

	std::string frameBuffer(SCREEN_SIZE, ' ');

	double* zBuffer = new double[SCREEN_SIZE];
	std::fill_n(zBuffer, SCREEN_SIZE, 0.0);

	const Vec3 rotationVector(0.0, 5.0, 0.0);
	const Vec3 translationVector(0.0, -1.0, 6.0);
	const Vec3 lightingVector(1.0, -1.0, 1.0);

	ObjFileLoader objLoader;
	Mesh* mesh = objLoader.loadFromFile("350z.obj");

	while (true) {

		*mesh = rotateMesh(*mesh, rotationVector);
		Mesh translatedMesh = translateMesh(*mesh, translationVector);
		drawMesh(translatedMesh, MESH_SHADED, lightingVector, WIDTH, HEIGHT, FOV, frameBuffer, zBuffer);

		drawFrame(frameBuffer, WIDTH, HEIGHT);

		frameBuffer.clear();
		frameBuffer.insert(0, SCREEN_SIZE, ' ');
		std::fill_n(zBuffer, SCREEN_SIZE, 0.0);

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}