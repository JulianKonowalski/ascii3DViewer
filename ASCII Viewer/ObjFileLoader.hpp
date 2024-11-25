#pragma once

#include <vector>
#include <fstream>
#include "Vec3.hpp"
#include "Mesh.hpp"

class ObjFileLoader {
public:
	ObjFileLoader() : mVertices(nullptr), mNormals(nullptr), mFaces(nullptr) {}
	~ObjFileLoader() { closeObjFile();  cleanup(); }
	Mesh* loadFromFile(const std::string& filePath);

private:
	void openObjFile(const std::string& filePath);
	void closeObjFile(void);
	void setup(void);
	void cleanup(void);

	std::string getFlag(void);
	void resolveFlag(std::string flag);

	void skipLine(void);

	void loadVertex(void);
	void loadNormal(void);
	void loadFace(void);

	std::ifstream mObjFile;
	std::vector<Vec3>* mVertices;
	std::vector<Vec3>* mNormals;
	std::vector<Triangle>* mFaces;
};