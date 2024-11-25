#include <algorithm>
#include <iostream>
#include <thread>

#include "Graphics.hpp"
#include "Mesh.hpp"
#include "ObjFileLoader.hpp"

const int WIDTH = 400, HEIGHT = 200, FOV = 45, SCREEN_SIZE = WIDTH * HEIGHT;

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

	delete mesh;

	return 0;
}