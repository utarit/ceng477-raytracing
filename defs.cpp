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

float Vector3f::length() const {
	return sqrt(x*x + y*y+ z*z);
}

	Vector3f Vector3f::operator*(const Vector3f &rhs) const {
		return {x*rhs.x, y*rhs.y, z*rhs.z};
	}
