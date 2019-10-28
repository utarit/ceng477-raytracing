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
    return origin + direction * t;
}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
    //TODO: check if direction.x == 0
    if (direction.x) {
	    return (p.x - origin.x) / direction.x;
    }
    else if (direction.y) {
	    return (p.y - origin.y) / direction.y;
    }
    else if (direction.z) {
        return (p.z - origin.z / direction.z);
    }
    else {
        //not gonna happen
        return 0;
    }
    
}

