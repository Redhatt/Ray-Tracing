#include "globals.h"

#include "light.h"

Light::Light() : color(),
				 rIndex(1.0)
{
}

Light::Light(const Color &color, float rIndex) : color(color),
												 rIndex(rIndex)
{
}

Light::~Light()
{
}

Light &Light::operator=(const Light &l)
{
	color = l.color;
	return *this;
}

Light &Light::operator+=(const Light &l)
{
	color += l.color;
	return *this;
}

Light &Light::operator*=(const Light &l)
{
	color *= l.color;
	return *this;
}