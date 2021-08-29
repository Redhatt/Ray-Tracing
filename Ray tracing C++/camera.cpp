#include "globals.h"

#include "camera.h"

PerspectiveCamera::PerspectiveCamera(Point origin,
                                     Vector target, Vector upguide, float fov, float aspectRatio)
	: origin(origin)
{
	forward = (target - origin).normalized();
	right = cross(forward, upguide).normalized();
	up = cross(right, forward);

	h = tan(fov);
	w = h * aspectRatio;
}

Ray PerspectiveCamera::makeRay(Vector2 point) const
{
	Vector direction =
	    forward + point.u * w * right + point.v * h * up;

	return Ray(origin, direction.normalized());
}

Ray PerspectiveCamera::getRay(int x, int y, int pWidth, int pHeight) const
{
	return makeRay(Vector2(((2.0 * x) / pWidth) - 1.0, ((2.0 * y) / pHeight) - 1.0));
}

Ray PerspectiveCamera::getRays(float x, float y, int pWidth, int pHeight) const
{
	return makeRay(Vector2(((2.0 * x) / pWidth) - 1.0, ((2.0 * y) / pHeight) - 1.0));
}

Point &PerspectiveCamera::getOrigin()
{
	return this->origin;
}

void PerspectiveCamera::setOrigin(Point &origin)
{
	this->origin = origin;
}