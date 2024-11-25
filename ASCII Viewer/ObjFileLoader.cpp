#include "ObjFileLoader.hpp"

Mesh* ObjFileLoader::loadFromFile(const std::string& filePath) {
	this->openObjFile(filePath);
	this->setup();

	while (!mObjFile.eof()) { resolveFlag(getFlag()); }

	Mesh* mesh = new Mesh(*mFaces);

	this->closeObjFile();
	this->cleanup();

	return mesh;
}

void ObjFileLoader::openObjFile(const std::string& filePath) {
	mObjFile = std::ifstream(filePath, std::ifstream::in);
	if (!mObjFile.is_open()) { throw std::runtime_error("Failed to import .obj file"); }
}

void ObjFileLoader::closeObjFile(void) {
	if (mObjFile.is_open()) { mObjFile.close(); }
}

void ObjFileLoader::setup(void) {
	mVertices = new std::vector<Vec3>();
	mNormals = new std::vector<Vec3>();
	mFaces = new std::vector<Triangle>();
}

void ObjFileLoader::cleanup(void) {
	delete mVertices;
	delete mNormals;
	delete mFaces;
}

std::string ObjFileLoader::getFlag(void) {
	char character = mObjFile.get();
	std::string flag = "";
	while (!mObjFile.eof() && character != ' ') {
		flag += character;
		character = mObjFile.get();
	}
	return flag;
}

void ObjFileLoader::resolveFlag(std::string flag) {
	if (flag == "v") { loadVertex(); }
	else if (flag == "vn") { loadNormal(); }
	else if (flag == "f") { loadFace(); }
	else { skipLine(); }
}

void ObjFileLoader::skipLine(void) {
	while (!mObjFile.eof() && mObjFile.get() != '\n');
}

void ObjFileLoader::loadVertex(void) {
	double x, y, z;
	mObjFile >> x >> y >> z;
	mVertices->push_back(Vec3(x, y, z));
	skipLine();
}

void ObjFileLoader::loadNormal(void) {
	double x, y, z;
	mObjFile >> x >> y >> z;
	mNormals->push_back(Vec3(x, y, z));
	skipLine();
}

void ObjFileLoader::loadFace(void) {
	int vertexIndices[3], normalIndex;
	for (int i = 0; i < 3; ++i) {
		mObjFile >> vertexIndices[i];
		for (int j = 0; j < 2; ++j) { mObjFile.get(); }
		mObjFile >> normalIndex; //normal is the same for all face vertices
		mObjFile.get();
	}
	const Vec3& normal = mNormals->at(normalIndex - 1);
	Triangle face(
		mVertices->at(vertexIndices[2] - 1),
		mVertices->at(vertexIndices[1] - 1),
		mVertices->at(vertexIndices[0] - 1)
	);
	mFaces->push_back(face);
}