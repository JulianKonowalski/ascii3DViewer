#include "Vec2.h"
#include "Triangle2D.h"
#include "Triangle3D.h"

#include <iostream>
#include <thread>

const int WIDTH = 150, HEIGHT = 75;
const int SCREEN_SIZE = WIDTH * HEIGHT;
const int FOV_ANGLE = 45;

Vec3 rotate(Vec3 point, Vec3 rotation) {
	double yaw = rotation.z() / 180.0 * 3.1415;
	double pitch = rotation.y() / 180.0 * 3.1415;
	double roll = rotation.x() / 180.0 * 3.1415;

	double xRotated =
		std::cos(yaw) * std::cos(pitch) * point.x() +
		(std::cos(yaw) * std::sin(pitch) * std::sin(roll) - std::sin(yaw) * std::cos(roll)) * point.y() +
		(std::cos(yaw) * std::sin(pitch) * std::cos(roll) + std::sin(yaw) * std::sin(roll)) * point.z();

	double yRotated =
		std::sin(yaw) * std::cos(pitch) * point.x() +
		(std::sin(yaw) * std::sin(pitch) * std::sin(roll) + std::cos(yaw) * std::cos(roll)) * point.y() +
		(std::sin(yaw) * std::sin(pitch) * std::cos(roll) - std::cos(yaw) * std::sin(roll)) * point.z();

	double zRotated =
		std::sin(pitch) * point.x() * -1.0 +
		std::cos(pitch) * std::sin(roll) * point.y() +
		std::cos(pitch) * std::cos(roll) * point.z();

	return Vec3(xRotated, yRotated, zRotated);
}

Vec3 translate(Vec3 point, Vec3 offset) {
	return Vec3(point.x() + offset.x(), point.y() + offset.y(), point.z() + offset.z());
}

Vec3 project(Vec3 vector, int FOV) {
	double angleRadians = (FOV / 180.0) * 3.1415;
	double xProjected = vector.x();
	double yProjected = vector.y();
	if (vector.z() != 0.0) {
		double scalingFactor = vector.z() * std::tan(angleRadians / 2);
		xProjected /= scalingFactor;
		yProjected /= scalingFactor;
	}
	return Vec3(xProjected, yProjected, vector.z()); //z is passed as depth information
}

Vec2 toConsoleCoordinates(Vec3 coord, int width, int height) {
	return Vec2(
		(coord.x() + 1) / 2 * (WIDTH - 1),
		((-1 * coord.y()) + 1) / 2 * (HEIGHT - 1)
	);
}

Triangle3D createTriangle3D(void) {
	Vec3 vertices[3] = {
		Vec3(0.0, 1.0, 0.0),
		Vec3(-1.0, -1.0, 0.0),
		Vec3(1.0, -1.0, 0.0)
	};
	return Triangle3D(vertices);
}

void drawLineHorizontal(const Vec2& lineStart, const Vec2& lineEnd, char* buffer) {
	int x0 = (int)lineStart.x();
	int x1 = (int)lineEnd.x();
	int y0 = (int)lineStart.y();
	int y1 = (int)lineEnd.y();

	if (x0 > x1) {
		int tmp = x0;
		x0 = x1;
		x1 = tmp;

		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int dir = dy < 0 ? -1 : 1;
	dy *= dir;

	if (dx == 0) { return; }

	int y = y0;
	int D = 2 * dy - dx;

	for (int i = 0; i < dx + 1; ++i) {
		buffer[x0 + i + y * WIDTH] = '*';
		if (D >= 0) {
			y += dir;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}
}

void drawLineVertical(const Vec2& lineStart, const Vec2& lineEnd, char* buffer) {
	int x0 = (int)lineStart.x();
	int x1 = (int)lineEnd.x();
	int y0 = (int)lineStart.y();
	int y1 = (int)lineEnd.y();

	if (y0 > y1) {
		int tmp = x0;
		x0 = x1;
		x1 = tmp;

		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int dir = dx < 0 ? -1 : 1;
	dx *= dir;

	if (dy == 0) { return; }

	int x = x0;
	int D = 2 * dx - dy;

	for (int i = 0; i < dy + 1; ++i) {
		buffer[x + (y0 + i) * WIDTH] = '*';
		if (D >= 0) {
			x += dir;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}
}

void rasteriseLine(const Vec2& lineStart, const Vec2& lineEnd, char* buffer) {
	if (fabs(lineEnd.x() - lineStart.x()) > fabs(lineEnd.y() - lineStart.y())) {
		drawLineHorizontal(lineStart, lineEnd, buffer);
	} else {
		drawLineVertical(lineStart, lineEnd, buffer);
	}
}

void fillTriangle(const Vec2& v1, const Vec2& v2, const Vec2& v3, char* buffer) {
	double y1 = v1.y();
	double y2 = v2.y();
	double y3 = v3.y();

	double x1 = v1.x();
	double x2 = v2.x();
	double x3 = v3.x();

	int xMin = (int)std::min(std::min(x1, x2), x3);
	int xMax = (int)std::max(std::max(x1, x2), x3);
	int yMin = (int)std::min(std::min(y1, y2), y3);
	int yMax = (int)std::max(std::max(y1, y2), y3);

	for (int y = yMin; y <= yMax; y++) {
		for (int x = xMin; x <= xMax; x++) {
			if ((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) >= 0 &&
				(x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) >= 0 &&
				(x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) >= 0) {
				buffer[x + y * WIDTH] = '.';
			}
		}
	}
}

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

		Vec2 p1Console = toConsoleCoordinates(p1, WIDTH, HEIGHT);
		Vec2 p2Console = toConsoleCoordinates(p2, WIDTH, HEIGHT);
		Vec2 p3Console = toConsoleCoordinates(p3, WIDTH, HEIGHT);

		fillTriangle(p1Console, p2Console, p3Console, buffer);

		rasteriseLine(p1Console, p2Console, buffer);
		rasteriseLine(p2Console, p3Console, buffer);
		rasteriseLine(p3Console, p1Console, buffer);

		buffer[(int)p1Console.x() + (int)p1Console.y() * WIDTH] = '@';
		buffer[(int)p2Console.x() + (int)p2Console.y() * WIDTH] = '@';
		buffer[(int)p3Console.x() + (int)p3Console.y() * WIDTH] = '@';

		for (int i = 0; i < SCREEN_SIZE; ++i) {
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