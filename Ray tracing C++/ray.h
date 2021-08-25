#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "color.h"
#include "light.h"

#define RAY_T_MIN 0.0001
#define RAY_T_MAX 1.0e30

struct Ray
{
    Point origin;
    Vector direction;
    float tMax;

    Ray();
    Ray(const Ray &r);
    Ray(const Point &origin, const Vector &direction, float tMax = RAY_T_MAX);

    virtual ~Ray();

    Ray &operator=(const Ray &r);

    Point calculate(float t) const;
};

class Shape;
class ShapeSet;

struct Intersection
{
    Ray ray;
    float t;
    Shape *pShape;
    Light light;

    Intersection();
    Intersection(const Intersection &i);
    Intersection(const Ray &ray);

    virtual ~Intersection();

    Intersection &operator=(const Intersection &i);

    bool intersected() const;
    Point position() const;
    void getSurfaceLight(Vector &out, ShapeSet *shapes, int depth);
};

#endif