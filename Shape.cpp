#include "Shape.h"
#include "Scene.h"
#include <cstdio>


Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}


Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	radius = R;
	center = pVertices->at(cIndex - 1);
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	auto a = dotProduct(ray.direction, ray.direction); //ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
	Vector3f tmp_v = sub3f(ray.origin, center);
	auto b = 2 * dotProduct(ray.direction, tmp_v);
	auto c = dotProduct(tmp_v, tmp_v) - radius * radius;

	float delta = b * b - 4 * a * c;
	if (delta < 0) 
	{
		return {0, 0, 0};
	}
	else 
	{
		auto t = (-b - sqrt(delta)) / (2.0 * a);
		Vector3f point = ray.getPoint(t);
		Vector3f normal = sub3f(point, center);
		return {
			delta>0?2:1, point, normal
		};
		/*if (delta > 0) 
		{
			ReturnVal result = { 2, point, normal };
			return result;
		}
		else 
		{
			ReturnVal result = { 1, point, normal };
			return result;
		}*/
	}
		

}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	auto p1 = pVertices[p1Index - 1];
	auto p2 = pVertices[p2Index - 2];
	auto p3 = pVertices[p3Index - 3];
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}
