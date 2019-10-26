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
	Vector3f tmp_v = ray.origin- center;
	float b = 2 * ray.direction.dotProduct(tmp_v);
	float c = tmp_v.dotProduct(tmp_v) - radius * radius;

	float delta = b * b - 4 * a * c;
	if (delta < 0) 
	{
		return {0, 0, 0};
	}
	else 
	{
		float t = (-b - sqrt(delta)) / (2.0 * a);
		Vector3f point = ray.getPoint(t);
		Vector3f normal = (point- center);
		return {
			delta>0?2:1, point, normal, t
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
	v1 = pVertices->at(p1Index - 1);
	v2 = pVertices->at(p2Index - 1);
	v3 = pVertices->at(p3Index - 1);

	normal = (v2-v1).crossProduct(v3-v1);
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
	// Vector3f p;
	// Vector3f na = (v3-v2).crossProduct(p-v2);
	// Vector3f nb = (v1-v3).crossProduct(p-v3);
	// Vector3f nc = (v2-v1).crossProduct(p-v1);
	// float alpha = normal.dotProduct(na) / (normal.length() * normal.length());
	// float beta = normal.dotProduct(nb) / (normal.length() * normal.length());
	// float gamma = normal.dotProduct(nc) / (normal.length() * normal.length());

	float a = v1.x - v2.x;
	float b = v1.y - v2.y;
	float c = v1.z - v2.z;
	float d = v1.x - v3.x;
	float e = v1.y - v3.y;
	float f = v1.z - v3.z;
	float g = ray.direction.x;
	float h = ray.direction.y;
	float i = ray.direction.z;
	float j = v1.x - ray.origin.x;
	float k = v1.y - ray.origin.y;
	float l = v1.z - ray.origin.z;
	float eihf = e*i - h*f;
	float gfdi = g*f - d*i;
	float dheg = d*h - e*g;
	float akjb = a*k - j*b;
	float jcal = j*c - a*l;
	float blkc = b*l - k*c;

	float M = a*eihf + b*gfdi + c*dheg;
	float beta = (j*eihf+k*gfdi+l*dheg) / M;
	float gamma = (i*akjb +e*jcal + d*blkc) / M;
	float t = (f*akjb+e*jcal+d*blkc) / M;

	//if(t < 0 || t > 1) return {0}; // Tmin tmax'a bak
	if(gamma < 0 || gamma > 1) return {0};
	if(beta < 0 || beta > 1 - gamma) return {0};
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
		if(val.intersectionStatus == 1 && val.t < tmin){
			tmin = val.t;
			closestTriangle = &triangle;
			minVal = val;
		}
	}

	return minVal;

}
