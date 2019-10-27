#ifndef _DEFS_H_
#define _DEFS_H_

#include "math.h"
#include <iostream>
class Scene;


#define print(name, vector) std::cout << name << ": " << vector.r << ", " << vector.g << ", " << vector.b << std::endl
/* Structure to hold return value from ray intersection routine. 
This should hold information related to the intersection point, 
for example, coordinate of the intersection point, surface normal at the intersection point etc. 
Think about the variables you will need for this purpose and declare them here inside of this structure. */


/* 3 dimensional vector holding floating point numbers.
Used for both coordinates and color. 
Use x, y, z for coordinate computations, and use r, g, b for color computations. 
Note that you do not have to use this one if you use any vector computation library like Eigen. */
typedef struct Vector3f
{
	union 
	{
		float x;
		float r;
	};
	union
	{
		float y;
		float g;
	};
	union
	{
		float z;
		float b;
	};
	Vector3f operator- (const Vector3f &rhs)const;
	Vector3f operator+ (const Vector3f &rhs)const;
	Vector3f operator* (const int &t) const;
	Vector3f operator* (const float &t) const;
	Vector3f operator*(const Vector3f &rhs) const;
	void operator= (const Vector3f &rhs);
	float length() const;
	float dotProduct(const Vector3f &rhs) const;
	Vector3f crossProduct(const Vector3f &rhs) const;

} Vector3f;

typedef struct ReturnVal
{
	int intersectionStatus; // 0:no intersection 1:One intersection 2:Two intersections
	Vector3f point;
	Vector3f normalVector;
	float t;

} ReturnVal;

//
// The global variable through which you can access the scene data
//

float determinant(const Vector3f &v1, const Vector3f &v2, const Vector3f &v3);

extern Scene* pScene;

#endif
