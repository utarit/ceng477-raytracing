#include "defs.h"


Vector3f Vector3f::operator+ (const Vector3f &rhs)const{
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}
Vector3f Vector3f::operator- (const Vector3f &rhs)const{
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}
void Vector3f::operator= (const Vector3f &rhs){ 
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
Vector3f Vector3f::operator* (const int &t) const{
	return {t*x, t*y, t*z};
}
Vector3f Vector3f::operator* (const float &t) const{
	return {t*x, t*y, t*z};
}
float Vector3f::dotProduct(const Vector3f &rhs) const {
		return rhs.x * x + rhs.y * y + rhs.z * z;
	}
Vector3f Vector3f::crossProduct(const Vector3f &rhs) const {
		return {
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		};
    }
bool Vector3f::operator== (const float &rhs) const{
	return x == rhs && y == rhs && z == rhs;
}

bool Vector3f::operator== (const int &rhs) const{
	return x == rhs && y == rhs && z == rhs;
}
bool Vector3f::operator== (const Vector3f &rhs) const{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vector3f::operator!= (const int &rhs) const {
	return !(*this == rhs);
}
bool Vector3f::operator!= (const float &rhs) const {
	return !(*this == rhs);
}

float Vector3f::length() const {
	return sqrt(x*x + y*y+ z*z);
}

Vector3f Vector3f::operator*(const Vector3f &rhs) const {
	return {x*rhs.x, y*rhs.y, z*rhs.z};
}

float determinant(const Vector3f &v1, const Vector3f &v2, const Vector3f &v3) {
	return  v1.x * (v2.y * v3.z - v2.z * v3.y) + 
			-v1.y * (v2.x * v3.z - v2.z * v3.x) +
			v1.z * (v2.x * v3.y - v2.y * v3.x);
}

void Vector3f::normalize() {
	auto l = length();
	x /= l;
	y /= l;
	z /= l;
}