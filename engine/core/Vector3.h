#pragma once

#include <cmath> 

using namespace std;

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	Vector3 operator-(const Vector3& otherVec3) const{
		return Vector3(x - otherVec3.x, y - otherVec3.y, z - otherVec3.z);
	}

	Vector3 operator+(const Vector3& otherVec3) const {
		return Vector3(x + otherVec3.x, y + otherVec3.y, z + otherVec3.z);
	}

	void operator+=(const Vector3& otherVec3) {
		x += otherVec3.x;
		y += otherVec3.y;
		z += otherVec3.z;
	}

	Vector3 operator*(const float scalar) {
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	void normalize() {
		float dist = sqrt(z * z + x * x + y * y);

		if (dist > 0) {
			x /= dist;
			y /= dist;
			z /= dist;
		}
	}
};