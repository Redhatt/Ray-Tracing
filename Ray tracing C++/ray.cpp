#include "globals.h"

#include "ray.h"
#include "shape.h"
#include "color.h"

Ray::Ray()
	: origin(0.0, 0.0, 0.0),
	  direction(),
	  tMax(RAY_T_MAX)
{
}

Ray::Ray(const Ray &r)
	: origin(r.origin),
	  direction(r.direction),
	  tMax(r.tMax)
{
}

Ray::Ray(const Point &origin, const Vector &direction, float tMax)
	: origin(origin),
	  direction(direction),
	  tMax(tMax)
{
}

Ray::~Ray()
{
}

Ray &Ray::operator=(const Ray &r)
{
	origin = r.origin;
	direction = r.direction;
	tMax = r.tMax;
	return *this;
}

Point Ray::calculate(float t) const
{
	return origin + (direction * t);
}

Intersection::Intersection()
	: ray(),
	  t(RAY_T_MAX),
	  pShape(NULL),
	  light(Color(0.0, 0.0, 1.0), 1.0f)
{
}

Intersection::Intersection(const Intersection &i)
	: ray(i.ray),
	  t(i.t),
	  pShape(i.pShape),
	  light(Color(0.0, 0.0, 1.0), 1.0f)
{
}

Intersection::Intersection(const Ray &ray)
	: ray(ray),
	  t(ray.tMax),
	  pShape(NULL),
	  light(Color(0.0, 0.0, 1.0), 1.0f)
{
}

Intersection::~Intersection()
{
}

Intersection &Intersection::operator=(const Intersection &i)
{
	ray = i.ray;
	t = i.t;
	pShape = i.pShape;
	return *this;
}

Point Intersection::position() const
{
	return ray.calculate(t);
}

void Intersection::getSurfaceLight(Vector &out, ShapeSet *shapes, int depth, bool inside)
{
	if (pShape != NULL)
	{
		light = pShape->light(ray.calculate(t), ray.origin, out, depth, inside, shapes);
	}
}