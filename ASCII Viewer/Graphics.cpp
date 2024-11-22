#include "Graphics.h"

Vec3 graphics::rotate(const Vec3& point, const Vec3& rotation) {
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

Vec3 graphics::translate(const Vec3& point, const Vec3& offset) {
	return Vec3(point.x() + offset.x(), point.y() + offset.y(), point.z() + offset.z());
}

Vec3 graphics::project(const Vec3& vector, const int FOV) {
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

Vec3 graphics::toConsoleCoordinates(const Vec3& worldCoord, const int screenWidth, const int height) {
	return Vec3(
		(worldCoord.x() + 1) / 2 * (screenWidth - 1),
		((-1 * worldCoord.y()) + 1) / 2 * (height - 1)
	);
}

void drawLineHorizontal(const Vec3& lineStart, const Vec3& lineEnd, int screenWidth, char* buffer) {//helper function
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
		buffer[x0 + i + y * screenWidth] = '*';
		if (D >= 0) {
			y += dir;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}
}

void drawLineVertical(const Vec3& lineStart, const Vec3& lineEnd, int screenWidth, char* buffer) { //helper function
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
		buffer[x + (y0 + i) * screenWidth] = '*';
		if (D >= 0) {
			x += dir;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}
}

void graphics::drawLine(const Vec3& lineStart, const Vec3& lineEnd, int screenWidth, char* buffer) {
	if (fabs(lineEnd.x() - lineStart.x()) > fabs(lineEnd.y() - lineStart.y())) {
		drawLineHorizontal(lineStart, lineEnd, screenWidth, buffer);
	}
	else {
		drawLineVertical(lineStart, lineEnd, screenWidth, buffer);
	}
}

void graphics::fillTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3, int screenWidth, char* buffer) {
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
				buffer[x + y * screenWidth] = '.';
			}
		}
	}
}