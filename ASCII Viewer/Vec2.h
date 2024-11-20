#pragma once

#include <iostream>

class Vec2 {
public:
	Vec2(void) : mX(0.0f), mY(0.0f) {}
	Vec2(double a, double b) : mX(a), mY(b) {}
	Vec2(const Vec2& v) : mX(v.mX), mY(v.mY) {}

	double x(void) const { return mX; }
	double y(void) const { return mY; }

	void add(Vec2 v) { mX += v.mX; mY += v.mY; }

	friend std::ostream& operator<<(std::ostream& os, const Vec2& v) { return os << v.x() << " " << v.y() << "\n"; };

private:
	double mX, mY;
};