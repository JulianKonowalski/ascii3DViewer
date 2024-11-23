#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec3>& indices) : mFaces(std::vector<Triangle>(indices.size())) {
	int indicesSize = indices.size();
	int facesSize = mFaces.size();
	for (int i = 0; i < indicesSize; ++i) {
		mFaces[i] = Triangle(				//indices determine which vertices describe a face (triangle)
			vertices[(int)indices[i].x() - 1], 
			vertices[(int)indices[i].y() - 1],
			vertices[(int)indices[i].z() - 1]
		);
	}
}