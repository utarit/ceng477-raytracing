#include "defs.h"


Vector3f Vector3f::operator+ (Vector3f &rhs){
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}
Vector3f Vector3f::operator- (Vector3f &rhs){
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}
Vector3f Vector3f::operator= (Vector3f &rhs){    
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}

float Vector3f::dotProduct(Vector3f &rhs) {
		return rhs.x * x + rhs.y * y + rhs.z * z;
	}
Vector3f crossProduct(Vector3f &rhs) {
		return ;
    }