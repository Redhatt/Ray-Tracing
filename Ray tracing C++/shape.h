#ifndef SHAPE_H
#define SHAPE_H

#include "globals.h"

#include "materials.cpp"
#include "vector.h"
#include "ray.h"
#include "color.h"
#include "light.h"


struct TreeData
{
    int start = -1;
    int end = -1;
    Vector left;
    Vector right;
};

inline std::ostream &operator<<(std::ostream &os, TreeData &t)
{
    os << "start: " << t.start << "\nend  : " << t.end << "\nleft : " << t.left << "\nright: " << t.right << endl;
    return os;
}

class Shape
{
public:
    int maxDepth = 1;
    virtual ~Shape() {}

    virtual bool intersect( Intersection &intersection) = 0;
    virtual bool doesIntersect(const Ray &ray) = 0;
    virtual Light light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes) = 0;
    virtual Point getPoint() = 0;
    virtual Material &getMaterial() = 0;
    virtual Vector maxAlong(const Vector &axis) = 0;
};

class ShapeSet : public Shape
{
public:
    ShapeSet();
    std::vector<Shape *> shapes;
    std::vector<Shape *> lights;
    int minSize = 1;
    TreeData tree[36000];
    long long int calls = 0.0;
    virtual ~ShapeSet();

    void addShape(Shape *shape);
    void addLight(Shape *shape);
    virtual bool intersect(Intersection &intersection);
    virtual bool doesIntersect(const Ray &ray);
    virtual Light light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes);
    virtual bool intersectLight(Intersection &intersection);
    virtual bool doesIntersectLight(const Ray &ray);
    virtual Point getPoint();
    virtual Material &getMaterial();
    virtual Vector maxAlong(const Vector &axis);
    virtual void buildTree(int node, int low, int high);
    virtual int setBounds(int node, int low, int high);
    virtual bool doesIntersectBound(const Ray &ray, float &t, const Vector &v1, const Vector &v2);
    virtual bool intersectTree(int node, Intersection &intersection);
    virtual bool doesBVCollide(const Vector &v1, const Vector &v2, const Vector &v3, const Vector &v4);
    virtual void printTree();
};

class Plane : public Shape
{

protected:
    Point position;
    Vector normal;
    Material material;
    float radius = 5e1;

public:
    Plane(const Point &position, const Vector &normal);
    virtual ~Plane();

    virtual bool intersect( Intersection &intersection);
    virtual bool doesIntersect(const Ray &ray);
    virtual Light light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes);
    virtual void setMaterial(Material &material);
    virtual Material &getMaterial();
    virtual Point getPoint();
    virtual Vector getNormal(const Point& point);
    virtual Vector maxAlong(const Vector &axis);
    virtual void setRadius(float radius);
    virtual float getRadius();

};

class Sphere : public Shape
{
protected:
    Point center;
    float radius;
    Material material;

public:
    Sphere(const Point &center, const float radius);
    virtual ~Sphere();

    virtual bool intersect( Intersection &intersection);
    virtual bool doesIntersect(const Ray &ray);
    virtual Vector getNormal(const Point &point);
    virtual Light light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes);
    virtual void setMaterial(Material &material);
    virtual Material &getMaterial();
    virtual Point getPoint();
    virtual Vector maxAlong(const Vector &axis);
};

class Triangle : public Shape
{

protected:
    Point point1, point2, point3;
    Vector normal;
    Material material;
    Point center;

public:
    Triangle(const Point& point1, const Point& point2, const Point& point3);
    virtual ~Triangle();

    virtual bool intersect( Intersection &intersection);
    virtual bool doesIntersect(const Ray &ray);
    virtual Light light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes);
    virtual Point getPoint();
    virtual Point updatePoint();
    virtual Material &getMaterial();
    virtual void setMaterial(Material &material);
    virtual bool PointInTriangle (const Point &pt);
    virtual Vector getNormal(const Point& point);
    virtual Vector maxAlong(const Vector &axis);
};

class Polygon : public Shape
{
protected:
    vector<Point> points;
    Vector normal;
    Material material;
    Point center;

public:
    Polygon (const vector<Point> &points);
    virtual ~Polygon();

    virtual bool intersect( Intersection &intersection);
    virtual bool doesIntersect(const Ray &ray);
    virtual Light light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes);
    virtual Point getPoint();
    virtual Point updatePoint();
    virtual Material &getMaterial();
    virtual void setMaterial(Material &material);
    virtual bool PointInPolygon (const Point &pt);
    virtual Vector getNormal(const Point& point);
    virtual Vector maxAlong(const Vector &axis);
};

#endif