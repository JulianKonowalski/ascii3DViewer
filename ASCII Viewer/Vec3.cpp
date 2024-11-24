#include "Vec3.hpp"

Vec3 Vec3::operator +=(const Vec3& vector) {
	*this = *this + vector;
	return *this;
}

Vec3 Vec3::operator -=(const Vec3& vector) {
	*this = *this - vector;
	return *this;
}

Vec3 Vec3::operator *=(const double& scalar) {
	*this = *this * scalar;
	return *this;
}

Vec3 Vec3::operator /=(const double& scalar) {
	*this = *this / scalar;
	return *this;
}

/* UTILITY */

Vec3 operator+(const Vec3& v1, const Vec3& v2) { return Vec3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()); }
Vec3 operator-(const Vec3& v1, const Vec3& v2) { return Vec3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()); }
Vec3 operator*(const Vec3& v, const double& scalar) { return Vec3(v.x() * scalar, v.y() * scalar, v.z() * scalar); }
Vec3 operator/(const Vec3& v, const double& scalar) { return Vec3(v.x() / scalar, v.y() / scalar, v.z() / scalar); }
bool operator==(const Vec3& v1, const Vec3& v2) { return (fabs(v1.x() - v1.x()) < 0.0001 && fabs(v1.y() - v2.y()) < 0.0001 && fabs(v1.z() - v2.z()) < 0.0001); }
bool operator!=(const Vec3& v1, const Vec3& v2) { return (fabs(v1.x() - v1.x()) > 0.0001 || fabs(v1.y() - v2.y()) > 0.0001 || fabs(v1.z() - v2.z()) > 0.0001); }

double vector3::lengthSquared(const Vec3& vector) {
	return (
		vector.x() * vector.x() +
		vector.y() * vector.y() +
		vector.z() * vector.z()
	);
}

double vector3::length(const Vec3& vector) {
	return sqrt(lengthSquared(vector));
}

Vec3 vector3::normalize(const Vec3& vector) {
	return vector / length(vector);
}

double vector3::dotProduct(const Vec3& v1, const Vec3& v2) {
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

Vec3 vector3::minX(const Vec3& v1, const Vec3& v2) {
	if (v1.x() < v2.x()) { return v1; }
	return v2;
}

Vec3 vector3::minY(const Vec3& v1, const Vec3& v2) {
	if (v1.y() < v2.y()) { return v1; }
	return v2;
}

Vec3 vector3::minZ(const Vec3& v1, const Vec3& v2) {
	if (v1.z() < v2.z()) { return v1; }
	return v2;
}

Vec3 vector3::maxX(const Vec3& v1, const Vec3& v2) {
	if (v1.x() > v2.x()) { return v1; }
	return v2;
}

Vec3 vector3::maxY(const Vec3& v1, const Vec3& v2) {
	if (v1.y() > v2.y()) { return v1; }
	return v2;
}

Vec3 vector3::maxZ(const Vec3& v1, const Vec3& v2) {
	if (v1.z() > v2.z()) { return v1; }
	return v2;
}