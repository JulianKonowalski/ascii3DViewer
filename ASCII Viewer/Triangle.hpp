#pragma once

#include "Vec3.hpp"

class Triangle {
public:
	Triangle(const Vec3* vertices) {
		for (int i = 0; i < 3; ++i) { mVertices[i] = vertices[i]; }
		calculateNormal();
	}

	Triangle(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
		mVertices[0] = v1;
		mVertices[1] = v2;
		mVertices[2] = v3;
		calculateNormal();
	}

	Triangle() {
		mVertices[0] = Vec3();
		mVertices[1] = Vec3();
		mVertices[2] = Vec3();
		calculateNormal();
	}

	const Vec3* getVertices(void) const { return mVertices; }
	Vec3 p1(void) const { return mVertices[0]; }
	Vec3 p2(void) const { return mVertices[1]; }
	Vec3 p3(void) const { return mVertices[2]; }
	Vec3 normal(void) const { return mNormal; }

	void setP1(const Vec3& point) { mVertices[0] = point; calculateNormal();}
	void setP2(const Vec3& point) { mVertices[1] = point; calculateNormal();}
	void setP3(const Vec3& point) { mVertices[2] = point; calculateNormal();}

private:
	void calculateNormal(void) {
		Vec3 A = this->p1() - this->p2();
		Vec3 B = this->p3() - this->p1();
		mNormal = Vec3(
			A.y() * B.z() - A.z() * B.y(),
			A.z() * B.x() - A.x() * B.z(),
			A.x() * B.y() - A.y() * B.x()
		);
	}

	Vec3 mVertices[3];
	Vec3 mNormal;
};