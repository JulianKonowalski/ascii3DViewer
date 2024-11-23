#ifndef VEC3_H
#define VEC3_H

#include <iostream>

class Vec3 {
public:
	Vec3(void) : mX(0.0f), mY(0.0f), mZ(0.0f) {}
	Vec3(double x, double y, double z) : mX(x), mY(y), mZ(z) {}
	Vec3(const Vec3& v) : mX(v.mX), mY(v.mY), mZ(v.mZ) {}

	double x(void) const { return mX; }
	double y(void) const { return mY; }
	double z(void) const { return mZ; }

	Vec3 operator +=(const Vec3& vector);
	Vec3 operator -=(const Vec3& vector);
	Vec3 operator *=(const double& scalar);
	Vec3 operator /=(const double& scalar);

	friend std::ostream& operator<<(std::ostream& os, const Vec3& v) { return os << v.x() << " " << v.y() << " " << v.z() << "\n"; };

private:
	double mX, mY, mZ;
};

Vec3 operator+(const Vec3& v1, const Vec3& v2);
Vec3 operator-(const Vec3& v1, const Vec3& v2);
Vec3 operator*(const Vec3& v, const double& scalar);
Vec3 operator/(const Vec3& v, const double& scalar);
bool operator==(const Vec3& v1, const Vec3& v2);
bool operator!=(const Vec3& v1, const Vec3& v2);

/* UTILITY */
namespace vector3 {

	double lengthSquared(const Vec3& vector);
	double length(const Vec3& vector);
	Vec3 normalize(const Vec3& vector);

	double dotProduct(const Vec3& v1, const Vec3& v2);
}

#endif // !VEC3_H