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
	float a = ray.direction.dotProduct(ray.direction); //dotProduct(ray.direction, ray.direction); //ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
	Vector3f tmp_v = ray.origin - center;
	float b = ray.direction.dotProduct(tmp_v);
	float c = tmp_v.dotProduct(tmp_v) - radius * radius;

	auto delta = 4 * b * b - 4 * a * c;
	if (delta < 0) 
	{
		return {0, 0, 0};
	}

	else 
	{
		auto sq = sqrt(b*b - a*c);
		auto t_1 = (-b + sq) / a;
		auto t_2 = (-b - sq) / a;

		if (t_1 <= pScene->intTestEps && t_2 <= pScene->intTestEps) {
			return {0, 0, 0, 0.0f};
		}
		auto t = t_1 > pScene->intTestEps ?
					(t_2 > pScene->intTestEps ?
						min(t_1, t_2):
						t_1):
					t_2;
	
		Vector3f point = ray.getPoint(t);
		Vector3f normal = (point - center) * (1 / radius);

		return {
			1, point, normal, t
		};
	}
		

}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	v1 = pVertices->at(p1Index - 1);
	v2 = pVertices->at(p2Index - 1);
	v3 = pVertices->at(p3Index - 1);

	ac = v1 - v3;
	ab = v1 - v2;
	normal = (v2-v1).crossProduct(v3-v1);
	normal = normal * (1/normal.length());
	area = 1/2 * (normal.length());
}

// float Triangle::sign (Vector3f p1, Vector3f p2, Vector3f p3){
// 	    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
// }

// bool Triangle::checkIntersection (Vector3f pt)
// {
//     float d1, d2, d3;
//     bool has_neg, has_pos;

//     d1 = sign(pt, v1, v2);
//     d2 = sign(pt, v2, v3);
//     d3 = sign(pt, v3, v1);

//     has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
//     has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

//     return !(has_neg && has_pos);
// }

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	
	auto ao = v1 - ray.origin;
	auto A = determinant(ab, ac, ray.direction);
	auto B = determinant(ao, ac, ray.direction) / A;
	auto G = determinant(ab, ao, ray.direction) / A;
	auto t = determinant(ab, ac, ao) / A;
	
	//std::cout << A << " " << B << " " << G << " " << t << std::endl;
	if (t < pScene->intTestEps || G < 0 || B < 0 || G + B > 1){
		return {0,0,0};
	}
	
	
	Vector3f point = ray.origin + ray.direction*t;
	ReturnVal result = {1, point, normal, t};
	return result;

}

Mesh::Mesh(){}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	// triangles = faces;
	for(int i = 0; i < faces.size(); i++){
		triangles.push_back(faces.at(i));
	}

}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	float tmin = (unsigned) -1;
	Triangle *closestTriangle = NULL;
	ReturnVal minVal = {0};
	for(int i = 0; i < triangles.size(); i++){
		Triangle triangle = triangles.at(i);
		ReturnVal val =  triangle.intersect(ray);
		if(val.intersectionStatus > 0 && val.t < tmin){
			tmin = val.t;
			closestTriangle = &triangle;
			minVal = val;
		}
	}

	return minVal;

}
