#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"

// during any operation the rIndex of first argument will be passed only

struct Light
{
	Color color;
	float rIndex;

	Light();
	Light(const Color &color, float rIndex);

	virtual ~Light();

	Light &operator=(const Light &l);
	Light &operator+=(const Light &l);
	Light &operator*=(const Light &l);
	Light &operator*=(float f);
};

inline Light operator+(const Light &c1, const Light &c2)
{
	Color c = c1.color + c2.color;
	return Light(c, c1.rIndex);
}

inline Light operator*(const Light &c1, const Light &c2)
{
	Color c = c1.color * c2.color;
	return Light(c, c1.rIndex);
}

inline Light operator*(const Light &c, float f)
{
	Color c1 = c.color * f;
	return Light(c1, c.rIndex);
}

#endif