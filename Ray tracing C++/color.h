#ifndef COLOR_H
#define COLOR_H

#include "globals.h"

struct Color
{
	float r, g, b;

	Color();
	Color(float l);
	Color(float r, float g, float b);
	Color(double r, double g, double b);
	Color(int r, int g, int b);

	virtual ~Color();

	void clamp(float min = 0.0f, float max = 1.0f);
	void applyGammaCorrection(float exposure, float gamma);

	Color &operator=(const Color &c);
	Color &operator+=(const Color &c);
	Color &operator+=(float f);
	Color &operator*=(const Color &c);
	Color &operator*=(float f);
};

inline Color operator+(const Color &c1, const Color &c2)
{
	return Color(c1.r + c2.r,
				 c1.g + c2.g,
				 c1.b + c2.b);
}

inline Color operator*(const Color &c1, const Color &c2)
{
	return Color(c1.r * c2.r,
				 c1.g * c2.g,
				 c1.b * c2.b);
}

inline Color operator*(const Color &c, float f)
{
	return Color(c.r * f,
				 c.g * f,
				 c.b * f);
}

inline Color operator*(float f, const Color &c)
{
	return Color(f * c.r,
				 f * c.g,
				 f * c.b);
}

inline std::istream &operator>>(std::istream &is, Color &c)
{
	is >> c.r >> c.g >> c.b;
	return is;
}

inline std::ostream &operator<<(std::ostream &os, Color &c)
{
	os << c.r << " " << c.g << " " << c.b;
	return os;
}

#endif // COLOR_H
