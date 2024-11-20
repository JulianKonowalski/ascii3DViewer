#pragma once

#include <iostream>

class Vec3 {
public:
	Vec3(void) : mX(0.0f), mY(0.0f), mZ(0.0f) {}
	Vec3(double a, double b, double c) : mX(a), mY(b), mZ(c) {}
	Vec3(const Vec3& v) : mX(v.mX), mY(v.mY), mZ(v.mZ) {}

	double x(void) const { return mX; }
	double y(void) const { return mY; }
	double z(void) const { return mZ; }

	friend std::ostream& operator<<(std::ostream& os, const Vec3& v) { return os << v.x() << " " << v.y() << " " << v.z() << "\n"; };

private:
	double mX, mY, mZ;
};