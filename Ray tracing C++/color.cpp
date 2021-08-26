#include "globals.h"

#include "color.h"

Color::Color()
    : r(0.0), g(0.0), b(0.0)
{
}

Color::Color(float l)
    : r(l), g(l), b(l)
{
}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b)
{
}

Color::Color(double r, double g, double b)
    : r(float(r)), g(float(g)), b(float(b))
{
}

Color::Color(int r, int g, int b)
    : r(float(r) / 255), g(float(g) / 255), b(float(b) / 255)
{
}

Color::~Color()
{
}

void Color::clamp(float min, float max)
{
    r = std::max(min, std::min(max, r));
    g = std::max(min, std::min(max, g));
    b = std::max(min, std::min(max, b));
}

void Color::applyGammaCorrection(float exposure, float gamma)
{
    r = std::pow(r * exposure, gamma);
    g = std::pow(g * exposure, gamma);
    b = std::pow(b * exposure, gamma);
}

Color &Color::operator=(const Color &c)
{
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
}

Color &Color::operator+=(const Color &c)
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

Color &Color::operator+=(float f)
{
    r += f;
    g += f;
    b += f;
    return *this;
}

Color &Color::operator*=(const Color &c)
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

Color &Color::operator*=(float f)
{
    r *= f;
    g *= f;
    b *= f;
    return *this;
}

inline uint8(float value) 
{
  return 0 > value ? 0 : (255 < value ? 255 : round(value));
}

Color hueRotate(const Color &color, float hue)
{   
  const float cosA = cos(hue * DEG);
  const float sinA = sin(hue * DEG);
  
  const float neo[3]  = {cosA + (1.0f - cosA) / 3,
                         (1.0f - cosA) / 3 - sqrt(1.0f / 3) * sinA,
                         (1.0f - cosA) / 3 + sqrt(1.0f / 3) * sinA};
  
  Color newColor(uint8((255 * color.r * neo[0]) + (255 * color.g * neo[1]) + (255 * color.b * neo[2])),
              uint8((255 * color.r * neo[2]) + (255 * color.g * neo[0]) + (255 * color.b * neo[1])),
              uint8((255 * color.r * neo[1]) + (255 * color.g * neo[2]) + (255 * color.b * neo[0])));

  return newColor;
}