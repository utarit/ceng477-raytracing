#include "Light.h"


/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
	Vector3f result;
	float distance = sqrt(pow((p.x - position.x), 2) + pow((p.y - position.y), 2) + pow((p.z - position.z), 2));
	result.r = intensity.r / pow(distance, 2);
	result.g = intensity.g / pow(distance, 2);
	result.b = intensity.b / pow(distance, 2);
	return result;
}
