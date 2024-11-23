#include "Graphics.h"

std::string graphics::getBuffer(const int& screenWidth, const int& screenHeight) {
	return std::string(screenWidth * screenHeight, ' ');
}

void graphics::clearBuffer(std::string& buffer, const int& screenWidth, const int& screenHeight) {
	buffer.clear();
	buffer.insert(0, screenWidth * screenHeight, ' ');
}

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

Vec3 graphics::projectPoint(Vec3 point, const int& screenWidth, const int& screenHeight, const int& FOV) {
	double angleRadians = (FOV / 180.0) * 3.1415;
	double xProjected = point.x();
	double yProjected = point.y();
	if (point.z() != 0.0) {
		double scalingFactor = point.z() * std::tan(angleRadians / 2);
		xProjected /= scalingFactor;
		yProjected /= scalingFactor;
	}
	return graphics::toConsoleCoordinates(Vec3(xProjected, yProjected, point.z()), screenWidth, screenHeight);	//z is passed as depth information
}

Triangle graphics::projectTriangle(Triangle triangle, const int& screenWidth, const int& screenHeight, const int& FOV) {
	return Triangle(
		graphics::projectPoint(triangle.p1(), screenWidth, screenHeight, FOV),
		graphics::projectPoint(triangle.p2(), screenWidth, screenHeight, FOV),
		graphics::projectPoint(triangle.p3(), screenWidth, screenHeight, FOV)
	);
}

Vec3 graphics::toConsoleCoordinates(const Vec3& coord, const int& screenWidth, const int& screenHeight) {
	return Vec3(
		(coord.x() + 1) / 2 * (screenWidth - 1),
		((-1 * coord.y()) + 1) / 2 * (screenHeight - 1),
		coord.z()									//z is passed as depth information
	);
}

void graphics::rasterisePoint(const Vec3& point, const int& screenWidth, std::string& buffer) {
	buffer[(int)point.x() + (int)point.y() * screenWidth] = '@';
}

void rasteriseLineHorizontal(const Vec3& lineStart, const Vec3& lineEnd, const int& screenWidth, std::string& buffer) {//helper function
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

void rasteriseLineVertical(const Vec3& lineStart, const Vec3& lineEnd, const int& screenWidth, std::string& buffer) { //helper function
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

void graphics::rasteriseLine(const Vec3& lineStart, const Vec3& lineEnd, const int& screenWidth, std::string& buffer) {
	if (fabs(lineEnd.x() - lineStart.x()) > fabs(lineEnd.y() - lineStart.y())) {
		rasteriseLineHorizontal(lineStart, lineEnd, screenWidth, buffer);
	}
	else {
		rasteriseLineVertical(lineStart, lineEnd, screenWidth, buffer);
	}
}

void fillTriangle(const Triangle& triangle, const char& shade, const int& screenWidth, std::string& buffer) {
	double y1 = triangle.p1().y();
	double y2 = triangle.p2().y();
	double y3 = triangle.p3().y();	
	
	double x1 = triangle.p1().x();
	double x2 = triangle.p2().x();
	double x3 = triangle.p3().x();

	int xMin = (int)std::min(std::min(x1, x2), x3);
	int xMax = (int)std::max(std::max(x1, x2), x3);
	int yMin = (int)std::min(std::min(y1, y2), y3);
	int yMax = (int)std::max(std::max(y1, y2), y3);

	for (int y = yMin; y <= yMax; y++) {
		for (int x = xMin; x <= xMax; x++) {
			if ((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) >= 0 &&
				(x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) >= 0 &&
				(x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) >= 0) {
				buffer[x + y * screenWidth] = shade;
			}
		}
	}
}

void outlineTriangle(const Triangle& triangle, const int& screenWidth, std::string& buffer) {
	graphics::rasteriseLine(triangle.p1(), triangle.p2(), screenWidth, buffer);
	graphics::rasteriseLine(triangle.p2(), triangle.p3(), screenWidth, buffer);
	graphics::rasteriseLine(triangle.p3(), triangle.p1(), screenWidth, buffer);
}

void triangleVertices(const Triangle& triangle, const int& screenWidth, std::string& buffer) {
	graphics::rasterisePoint(triangle.p1(), screenWidth, buffer);
	graphics::rasterisePoint(triangle.p2(), screenWidth, buffer);
	graphics::rasterisePoint(triangle.p3(), screenWidth, buffer);
}

void graphics::rasteriseTriangle(const Triangle& triangle, const drawTriangleMode& mode, const char& shade, const int& screenWidth, std::string& buffer) {
	switch (mode) {
	case TRIANGLE_FULL:
		fillTriangle(triangle, shade, screenWidth, buffer);
		outlineTriangle(triangle, screenWidth, buffer);
		triangleVertices(triangle, screenWidth, buffer);
		break;
	case TRIANGLE_OUTLINE:
		outlineTriangle(triangle, screenWidth, buffer);
		break;
	case TRIANGLE_VERTICES:
		triangleVertices(triangle, screenWidth, buffer);
		break;
	case TRIANGLE_INSIDE:
		fillTriangle(triangle, shade, screenWidth, buffer);
		break;
	}
}

void insertNewLines(std::string& buffer, const int& screenWidth, const int& screenHeight) {
	for (int i = 1; i < screenHeight; ++i) {
		buffer.insert(i * screenWidth + (i - 1), 1, '\n');
	}
}

void graphics::drawFrame(std::string& buffer, const int& screenWidth, const int& screenHeight) {
	insertNewLines(buffer, screenWidth, screenHeight);
	system("cls");
	std::cout << buffer;
}