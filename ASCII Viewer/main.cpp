#include "Graphics.h"
#include "Triangle.h"

#include <iostream>
#include <thread>

const int WIDTH = 150, HEIGHT = 75;
const int SCREEN_SIZE = WIDTH * HEIGHT;
const int FOV_ANGLE = 45;

Triangle createTriangle3D(void) {
	Vec3 vertices[3] = {
		Vec3(0.0, 1.0, 0.0),
		Vec3(-1.0, -1.0, 0.0),
		Vec3(1.0, -1.0, 0.0)
	};
	return Triangle(vertices);
}

using namespace graphics;

int main(void) {

	char buffer[SCREEN_SIZE];
	std::string bufferString;

	Triangle triangle = createTriangle3D();

	Vec3 rotationVector(0.0, 2.0, 1.0);
	Vec3 translationVector(0.0, 0.0, 5.0);

	while (true) {
		memset(buffer, ' ', SCREEN_SIZE);
	
		triangle.setP1(rotate(triangle.p1(), rotationVector));
		triangle.setP2(rotate(triangle.p2(), rotationVector));
		triangle.setP3(rotate(triangle.p3(), rotationVector));

		Vec3 p1 = project(translate(triangle.p1(), translationVector), FOV_ANGLE);
		Vec3 p2 = project(translate(triangle.p2(), translationVector), FOV_ANGLE);
		Vec3 p3 = project(translate(triangle.p3(), translationVector), FOV_ANGLE);

		Vec3 p1Console = toConsoleCoordinates(p1, WIDTH, HEIGHT);
		Vec3 p2Console = toConsoleCoordinates(p2, WIDTH, HEIGHT);
		Vec3 p3Console = toConsoleCoordinates(p3, WIDTH, HEIGHT);

		Triangle projectedTriangle(p1Console, p2Console, p3Console);

		fillTriangle(projectedTriangle, WIDTH, buffer);

		drawLine(p1Console, p2Console, WIDTH, buffer);
		drawLine(p2Console, p3Console, WIDTH, buffer);
		drawLine(p3Console, p1Console, WIDTH, buffer);

		drawPoint(p1Console, WIDTH, buffer);
		drawPoint(p2Console, WIDTH, buffer);
		drawPoint(p3Console, WIDTH, buffer);

		for (int i = 0; i < SCREEN_SIZE; ++i) { //change this
			if (i != 0 && !(i % WIDTH)) { bufferString += "\n"; }
			bufferString += buffer[i];
		}

		system("cls");
		std::cout << bufferString;
		bufferString.clear();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}