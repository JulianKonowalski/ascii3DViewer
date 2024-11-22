#include "Graphics.h"
#include "Triangle3D.h"

#include <iostream>
#include <thread>

const int WIDTH = 150, HEIGHT = 75;
const int SCREEN_SIZE = WIDTH * HEIGHT;
const int FOV_ANGLE = 45;

Triangle3D createTriangle3D(void) {
	Vec3 vertices[3] = {
		Vec3(0.0, 1.0, 0.0),
		Vec3(-1.0, -1.0, 0.0),
		Vec3(1.0, -1.0, 0.0)
	};
	return Triangle3D(vertices);
}

using namespace graphics;

int main(void) {

	char buffer[SCREEN_SIZE];
	std::string bufferString;

	Triangle3D triangle3d = createTriangle3D();

	Vec3 rotationVector(0.0, 2.0, 1.0);
	Vec3 translationVector(0.0, 0.0, 5.0);

	while (true) {
		memset(buffer, ' ', SCREEN_SIZE);
	
		triangle3d.setP1(rotate(triangle3d.p1(), rotationVector));
		triangle3d.setP2(rotate(triangle3d.p2(), rotationVector));
		triangle3d.setP3(rotate(triangle3d.p3(), rotationVector));

		Vec3 p1 = project(translate(triangle3d.p1(), translationVector), FOV_ANGLE);
		Vec3 p2 = project(translate(triangle3d.p2(), translationVector), FOV_ANGLE);
		Vec3 p3 = project(translate(triangle3d.p3(), translationVector), FOV_ANGLE);

		Vec3 p1Console = toConsoleCoordinates(p1, WIDTH, HEIGHT);
		Vec3 p2Console = toConsoleCoordinates(p2, WIDTH, HEIGHT);
		Vec3 p3Console = toConsoleCoordinates(p3, WIDTH, HEIGHT);

		fillTriangle(p1Console, p2Console, p3Console, WIDTH, buffer);

		drawLine(p1Console, p2Console, WIDTH, buffer);
		drawLine(p2Console, p3Console, WIDTH, buffer);
		drawLine(p3Console, p1Console, WIDTH, buffer);

		buffer[(int)p1Console.x() + (int)p1Console.y() * WIDTH] = '@';
		buffer[(int)p2Console.x() + (int)p2Console.y() * WIDTH] = '@';
		buffer[(int)p3Console.x() + (int)p3Console.y() * WIDTH] = '@';

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