#include "Graphics.hpp"
#include "Triangle.hpp"

#include <iostream>
#include <thread>

const int WIDTH = 150, HEIGHT = 75;
const int FOV = 45;

Triangle createTriangle(void) {
	return Triangle(
		Vec3(0.0, 1.0, 0.0), 
		Vec3(-1.0, -1.0, 0.0),
		Vec3(1.0, -1.0, 0.0)
	);
}

using namespace graphics;

int main(void) {

	std::string buffer = getBuffer(WIDTH, HEIGHT);

	Triangle triangle = createTriangle();

	Vec3 rotationVector(0.0, 10.0, 10.0);
	Vec3 translationVector(0.0, 0.0, 5.0);

	while (true) {
	
		triangle.setP1(rotate(triangle.p1(), rotationVector));
		triangle.setP2(rotate(triangle.p2(), rotationVector));
		triangle.setP3(rotate(triangle.p3(), rotationVector));

		Vec3 p1 = projectPoint(translate(triangle.p1(), translationVector), WIDTH, HEIGHT, FOV);
		Vec3 p2 = projectPoint(translate(triangle.p2(), translationVector), WIDTH, HEIGHT, FOV);
		Vec3 p3 = projectPoint(translate(triangle.p3(), translationVector), WIDTH, HEIGHT, FOV);

		char triangleShade = shadeTriangle(triangle, Vec3(0.0, 0.0, 1.0)); //there's global light, so shading after translation is ok
		rasteriseTriangle(Triangle(p1, p2, p3), TRIANGLE_INSIDE, triangleShade, WIDTH, buffer);

		drawFrame(buffer, WIDTH, HEIGHT);
		clearBuffer(buffer, WIDTH, HEIGHT);

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}