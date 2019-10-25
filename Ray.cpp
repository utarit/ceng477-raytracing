#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : origin(origin), direction(direction)
{
}

/* Takes a parameter t and returns the point accoring to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3f Ray::getPoint(float t) const 
{
    Vector3f result;
    result.x = (origin.x)+t*(direction.x);
    result.y = (origin.y)+t*(direction.y);
    result.z = (origin.z)+t*(direction.z);
    return result;
}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
	return (p.x-origin.x) / direction.x;
}

