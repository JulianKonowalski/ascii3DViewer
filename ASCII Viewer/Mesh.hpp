#pragma once

#include <vector>

#include "Vec3.hpp"
#include "Triangle.hpp"

class Mesh {
public:

	Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec3>& indices);
	Mesh(const std::vector<Triangle>& faces) : mFaces(faces) {}

	const std::vector<Triangle>& getFaces(void) const { return mFaces; }

private:
	std::vector<Triangle> mFaces;
};