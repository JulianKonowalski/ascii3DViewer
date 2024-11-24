 #include "Graphics.hpp"

Vec3 graphics::rotatePoint(const Vec3& point, const Vec3& rotation) {
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

Triangle graphics::rotateTriangle(const Triangle& triangle, const Vec3& rotation) {
	const Vec3* vertices = triangle.getVertices();
	Vec3 rotated[3];
	for (int i = 0; i < 3; ++i) {
		rotated[i] = graphics::rotatePoint(vertices[i], rotation);
	}
	return Triangle(rotated[0], rotated[1], rotated[2]);
}

Mesh graphics::rotateMesh(const Mesh& mesh, const Vec3& rotation) {
	std::vector<Triangle> rotated(mesh.getFaces().size());
	const std::vector<Triangle>& faces = mesh.getFaces();
	for (int i = 0; i < mesh.getFaces().size(); ++i) {
		rotated[i] = graphics::rotateTriangle(faces[i], rotation);
	}
	return Mesh(rotated);
}

Vec3 graphics::translatePoint(const Vec3& point, const Vec3& offset) {
	return Vec3(point.x() + offset.x(), point.y() + offset.y(), point.z() + offset.z());
}

Triangle graphics::translateTriangle(const Triangle& triangle, const Vec3& offset) {
	const Vec3* vertices = triangle.getVertices();
	Vec3 translated[3];
	for (int i = 0; i < 3; ++i) {
		translated[i] = graphics::translatePoint(vertices[i], offset);
	}
	return Triangle(translated[0], translated[1], translated[2]);
}

Mesh graphics::translateMesh(const Mesh& mesh, const Vec3& offset) {
	std::vector<Triangle> translated(mesh.getFaces().size());
	const std::vector<Triangle>& faces = mesh.getFaces();
	for (int i = 0; i < mesh.getFaces().size(); ++i) {
		translated[i] = graphics::translateTriangle(faces[i], offset);
	}
	return Mesh(translated);
}

Vec3 graphics::projectPoint(const Vec3& point, const int& screenWidth, const int& screenHeight, const int& FOV) {
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

Triangle graphics::projectTriangle(const Triangle& triangle, const int& screenWidth, const int& screenHeight, const int& FOV) {
	return Triangle(
		graphics::projectPoint(triangle.p1(), screenWidth, screenHeight, FOV),
		graphics::projectPoint(triangle.p2(), screenWidth, screenHeight, FOV),
		graphics::projectPoint(triangle.p3(), screenWidth, screenHeight, FOV)
	);
}

Mesh graphics::projectMesh(const Mesh& mesh, const int& screenWidth, const int& screenHeight, const int& FOV) {
	std::vector<Triangle> projected(mesh.getFaces().size());
	const std::vector<Triangle>& faces = mesh.getFaces();
	for (int i = 0; i < mesh.getFaces().size(); ++i) {
		projected[i] = graphics::projectTriangle(faces[i], screenWidth, screenHeight, FOV);
	}
	return Mesh(projected);
}

char graphics::shadeTriangle(const Triangle& triangle, const Vec3& lightDirection) {
	Vec3 triangleNormal = triangle.normal();
	double cosAngle = vector3::dotProduct(triangleNormal, lightDirection) / ( vector3::length(triangleNormal) * vector3::length(lightDirection) );
	double shadeIndex = -cosAngle * numberOfShades;
	shadeIndex = shadeIndex >= 0 ? shadeIndex : 0;
	return shades[(int)shadeIndex];
}

Vec3 graphics::toConsoleCoordinates(const Vec3& coord, const int& screenWidth, const int& screenHeight) {
	return Vec3(
		(coord.x() + 1) / 2 * (screenWidth - 1),
		((-1 * coord.y()) + 1) / 2 * (screenHeight - 1),
		coord.z()									//z is passed as depth information
	);
}

void graphics::rasterisePoint(const Vec3& point, const char& fill, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer) {
	if (point.x() < 0 || point.x() >= screenWidth - 1 || point.y() <= 0 || point.y() >= screenHeight - 1) { return; } //out of camera view

	int index = (int)point.x() + (int)point.y() * screenWidth;
	double x = zBuffer[index];
	if (zBuffer[index] == 0.0 || point.z() < zBuffer[index]) {
		frameBuffer[index] = fill;
		zBuffer[index] = point.z();
	}
}

void rasteriseLineHorizontal(const Vec3& lineStart, const Vec3& lineEnd, const char& fill, const int& screenWidth, std::string& frameBuffer, double* zBuffer) {//helper function
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
		frameBuffer[x0 + i + y * screenWidth] = fill;
		if (D >= 0) {
			y += dir;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}
}

void rasteriseLineVertical(const Vec3& lineStart, const Vec3& lineEnd, const char& fill, const int& screenWidth, std::string& frameBuffer, double* zBuffer) { //helper function
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
		frameBuffer[x + (y0 + i) * screenWidth] = fill;
		if (D >= 0) {
			x += dir;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}
}

void graphics::rasteriseLine(const Vec3& lineStart, const Vec3& lineEnd, const char& fill, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer) {
	if (fabs(lineEnd.x() - lineStart.x()) > fabs(lineEnd.y() - lineStart.y())) {
		rasteriseLineHorizontal(lineStart, lineEnd, fill, screenWidth, frameBuffer, zBuffer);
	}
	else {
		rasteriseLineVertical(lineStart, lineEnd, fill, screenWidth, frameBuffer, zBuffer);
	}
}

void triangleFill(const Triangle& triangle, const char& shade, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer) {
	double x1 = triangle.p1().x();
	double x2 = triangle.p2().x();
	double x3 = triangle.p3().x();

	double y1 = triangle.p1().y();
	double y2 = triangle.p2().y();
	double y3 = triangle.p3().y();

	Vec3 vxMin = vector3::minX(triangle.p1(), vector3::minX(triangle.p2(), triangle.p3()));
	Vec3 vxMax = vector3::maxX(triangle.p1(), vector3::maxX(triangle.p2(), triangle.p3()));
	Vec3 vyMin = vector3::minY(triangle.p1(), vector3::minY(triangle.p2(), triangle.p3()));
	Vec3 vyMax = vector3::maxY(triangle.p1(), vector3::maxY(triangle.p2(), triangle.p3()));

	//for point depth calculations
	double yz = vyMin.z();
	double xzDelta = (vxMax.z() - vxMin.z()) / (double)((int)vxMax.x() - (int)vxMin.x());
	double yzDelta = ( vyMin.z() - ( ( vxMin.z() + vxMax.z() ) / 2.0 ) ) / (vyMax.y() - vyMin.y());

	for (int y = (int)vyMin.y(); y <= (int)vyMax.y(); y++) {
		double xz = vxMin.z();
		for (int x = (int)vxMin.x(); x <= (int)vxMax.x(); x++) {
			if ((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) >= 0 &&
				(x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) >= 0 &&
				(x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) >= 0) {
				graphics::rasterisePoint(Vec3(x, y, xz + yz), shade, screenWidth, screenHeight, frameBuffer, zBuffer);
			}
			xz += xzDelta;
		}
		yz += yzDelta;
	}
}

void triangleOutline(const Triangle& triangle, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer) {
	graphics::rasteriseLine(triangle.p1(), triangle.p2(), '*', screenWidth, screenHeight, frameBuffer, zBuffer);
	graphics::rasteriseLine(triangle.p2(), triangle.p3(), '*', screenWidth, screenHeight, frameBuffer, zBuffer);
	graphics::rasteriseLine(triangle.p3(), triangle.p1(), '*', screenWidth, screenHeight, frameBuffer, zBuffer);
}

void triangleVertices(const Triangle& triangle, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer) {
	graphics::rasterisePoint(triangle.p1(), '@', screenWidth, screenHeight, frameBuffer, zBuffer);
	graphics::rasterisePoint(triangle.p2(), '@', screenWidth, screenHeight, frameBuffer, zBuffer);
	graphics::rasterisePoint(triangle.p3(), '@', screenWidth, screenHeight, frameBuffer, zBuffer);
}

void graphics::rasteriseTriangle(const Triangle& triangle, const drawTriangleMode& mode, const char& fill, const int& screenWidth, const int& screenHeight, std::string& frameBuffer, double* zBuffer) {
	switch (mode) {
	case TRIANGLE_FULL:
		triangleFill(triangle, fill, screenWidth, screenHeight, frameBuffer, zBuffer);
		triangleOutline(triangle, screenWidth, screenHeight, frameBuffer, zBuffer);
		triangleVertices(triangle, screenWidth, screenHeight, frameBuffer, zBuffer);
		break;
	case TRIANGLE_EDGES:
		triangleOutline(triangle, screenWidth, screenHeight, frameBuffer, zBuffer);
		break;
	case TRIANGLE_VERTICES:
		triangleVertices(triangle, screenWidth, screenHeight, frameBuffer, zBuffer);
		break;
	case TRIANGLE_SHADED:
		triangleFill(triangle, fill, screenWidth, screenHeight, frameBuffer, zBuffer);
		break;
	}
}

void graphics::drawMesh(const Mesh& mesh, const drawMeshMode& mode, const Vec3& lightDirection, const int& screenWidth, const int& screenHeight, const int& FOV, std::string& frameBuffer, double* zBuffer) {
	const std::vector<Triangle>& faces = mesh.getFaces();
	switch (mode) {
	case MESH_FULL:
		for (const Triangle& face : faces) {
			graphics::rasteriseTriangle(
				graphics::projectTriangle(face, screenWidth, screenHeight, FOV),
				TRIANGLE_FULL,
				graphics::shadeTriangle(face, lightDirection),
				screenWidth,
				screenHeight,
				frameBuffer,
				zBuffer
			);
		}
	case MESH_SHADED:
		for (const Triangle& face : faces) {
			graphics::rasteriseTriangle(
				graphics::projectTriangle(face, screenWidth, screenHeight, FOV), 
				TRIANGLE_SHADED, 
				graphics::shadeTriangle(face, lightDirection), 
				screenWidth, 
				screenHeight,
				frameBuffer,
				zBuffer
			);
		}
		break;
	case MESH_EDGES:
		for (const Triangle& face : faces) {
			graphics::rasteriseTriangle(
				graphics::projectTriangle(face, screenWidth, screenHeight, FOV),
				TRIANGLE_EDGES,
				' ',
				screenWidth,
				screenHeight,
				frameBuffer,
				zBuffer
			);
		}
		break;
	case MESH_VERTICES:
		for (const Triangle& face : faces) {
			graphics::rasteriseTriangle(
				graphics::projectTriangle(face, screenWidth, screenHeight, FOV),
				TRIANGLE_VERTICES,
				' ',
				screenWidth,
				screenHeight,
				frameBuffer,
				zBuffer
			);
		}
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