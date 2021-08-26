#include "globals.h"

#include "shape.h"
#include "matarials.h"
#include "vector.h"

Light sky(Color(169, 207, 245), 1.0);
// Light sky();

Light calculateLight(
    Point point,
    Point lastPoint,
    Vector income,
    Vector normal,
    Matarial matarial,
    ShapeSet *shapes,
    int depth,
    bool inside = false,
    float lightDiv = 0 * PI / 1000,
    float ambience = 0.95,
    int reflects = 1,
    int throughs = 1,
    int lights = 1,
    int shadow = 1)
{
    Vector out = -income;
    Light finalLight = Light();
    finalLight.color += (matarial.getEmission()/matarial.getEmissionFactor((point - lastPoint).length2()) + ambience) * 
                         matarial.getAmbient();

    // rays reflected

    if (matarial.getReflection() && !inside) {
        for (int i = 0; i < reflects; i++)
        {
            Vector reflectedVector = randomVector(2 * (dot(normal, out) * normal) - out,
                                                  matarial.getReflectDiv(),
                                                  float(rand()) / RAND_MAX);

            Intersection intersection(Ray(point, reflectedVector, 1e30));
            shapes->intersectTree(0, intersection);
            // shapes->intersectLight(intersection);

            intersection.getSurfaceLight(reflectedVector, shapes, inside, depth + 1);

            finalLight.color += ((1 - matarial.getAbsorb()) / float(reflects)) * intersection.light.color; //*
                                // std::max(0.0f, dot(normal, (reflectedVector + out).normalized()));
        }
    }

    // rays through refarction
    if (matarial.getRefraction())
    {
        for (int i = 0; i < throughs; i++)
        {
            float n = 1.0 / matarial.getRIndex();
            if (inside) {
                normal = -normal;
                n = matarial.getRIndex();
            }

            income.normalize();
            normal.normalize();

            float dott = dot(income, normal);
            float critical = 1 - sqr(n) * (1 - sqr(dott));
            Intersection intersection;

            if (critical < 0.0f) {
                // total internal reflection.
                Vector throughVector = 2 * (dot(normal, out) * normal) - out;
                
                intersection.ray = Ray(point, throughVector, 1e30);
                shapes->intersectTree(0, intersection);
                intersection.getSurfaceLight(throughVector, shapes, depth+1, inside);
            }
            else {
                Vector throughVector = n * (income - dott * normal) - 
                                       normal * sqrt(critical);

                intersection.ray = Ray(point, throughVector, 1e30);
                shapes->intersectTree(0, intersection);
                intersection.getSurfaceLight(throughVector, shapes, depth+1, !inside);
            }

            finalLight.color += intersection.light.color;
        }
    }

    // rays towards light
    if (matarial.getLight() && !inside) {
        for (std::vector<Shape *>::iterator iter = shapes->lights.begin();
             iter != shapes->lights.end(); iter++)
        {
            Shape *light = *iter;
            for (int i = 0; i < lights; i++)
            {
                Vector lightVector = randomVector(light->getPoint() - point,
                                                  lightDiv,
                                                  float(rand()) / RAND_MAX);

                Intersection intersection(Ray(point, lightVector, 1e30));
                
                shapes->intersectTree(0, intersection);

                if (intersection.pShape == light || light->intersect(intersection))
                {
                    finalLight.color += (light->getMatarial().getEmission() * (1 - matarial.getAbsorb()) / 
                                         (float(lights) * light->getMatarial().getEmissionFactor((light->getPoint() - point).length2()))) *
                                        light->getMatarial().getDiffuse() * matarial.getDiffuse() * std::max(0.0f, dot(normal, lightVector));

                    finalLight.color += (light->getMatarial().getEmission() * (1 - matarial.getAbsorb()) / 
                                          (float(lights) * light->getMatarial().getEmissionFactor((light->getPoint() - point).length2()))) *
                                        light->getMatarial().getSpecular() * matarial.getSpecular() * std::max(0.0f, dot(normal, (lightVector + out).normalized()));
                }
            }
        }
    }

    // rays to get ambient shadow
    if (matarial.getAmbienceShadow() && !inside)
    {
        for (int i=0; i<shadow; i++)
        {
            Vector shadowVector = randomVector(normal,
                                               PI/2,
                                               float(rand()) / RAND_MAX);

            Intersection intersection(Ray(point, shadowVector, 0.25));
            shapes->intersectTree(0, intersection);

            if (intersection.pShape != NULL && intersection.pShape->getMatarial().getEmission() == 0.0f)
            {
                finalLight.color *= 0.95;
            }
        }
    }

    return finalLight;
}

ShapeSet::ShapeSet()
{
}

ShapeSet::~ShapeSet()
{
}

void ShapeSet::addShape(Shape *shape)
{
    shapes.push_back(shape);
}

void ShapeSet::addLight(Shape *shape)
{
    lights.push_back(shape);
}

bool ShapeSet::intersect(Intersection &intersect)
{
    bool doesIntersect = false;

    for (std::vector<Shape *>::iterator iter = shapes.begin();
         iter != shapes.end();
         iter++)
    {
        calls += 1;
        Shape *curshape = *iter;
        if (curshape->intersect(intersect))
            doesIntersect = true;
    }

    return doesIntersect;
}

bool ShapeSet::doesIntersect(const Ray &ray)
{
    for (std::vector<Shape *>::iterator iter = shapes.begin();
         iter != shapes.end();
         iter++)
    {
        Shape *curShape = *iter;

        if (curShape->doesIntersect(ray))
            return true;
    }

    return false;
}

bool ShapeSet::intersectLight(Intersection &intersect)
{
    bool doesIntersect = false;

    for (std::vector<Shape *>::iterator iter = lights.begin();
         iter != lights.end();
         iter++)
    {
        Shape *curshape = *iter;
        if (curshape->intersect(intersect))
            doesIntersect = true;
    }

    return doesIntersect;
}

bool ShapeSet::doesIntersectLight(const Ray &ray)
{
    for (std::vector<Shape *>::iterator iter = lights.begin();
         iter != lights.end();
         iter++)
    {
        Shape *curShape = *iter;

        if (curShape->doesIntersect(ray))
            return true;
    }

    return false;
}

Point ShapeSet::getPoint()
{
    return Point();
}

Light ShapeSet::light(const Point &point, const Point &lastPoint, Vector &out, int depth, bool inside, ShapeSet *shapes)
{
    // nothing
    return sky;
}

Matarial ShapeSet::getMatarial()
{
    return Matarial();
}

bool ShapeSet::intersectTree(int node, Intersection &intersection)
{
    if (node >= 2*shapes.size() - 1) return false;

    float t;
    if (!doesIntersectBound(intersection.ray, t, tree[node].left, tree[node].right)) return false;
    
    // this is with below comment codes 196 line no: and below 221 line no: it didn't optimize rather increased calls
    // if ((node == 0) && (!doesIntersectBound(intersection.ray, t, tree[node].left, tree[node].right))) return false;

    bool check = false;
    if (tree[node].end - tree[node].start <= minSize) {
        for (int i=tree[node].start; i<=tree[node].end; i++) {
            calls += 1;
            if (shapes[i]->intersect(intersection)) {
                check = true;
            }
        }
        return check;
    }

    bool checkBoth = false;
    if (intersectTree(2*node + 1, intersection)) checkBoth = true;
    if (intersectTree(2*node + 2, intersection)) checkBoth = true;
    return checkBoth;

    /*
    if ray intersect with both childs then check if both childs bounding Volumes collides
    and if bounding Volumes collide then we have no choice but to check intersection in both childs
    otherwise we can priorities first intersecting child and return if intersection found in first because than
    we are sure there is no early intersection in second child.
    */

    // float t1=1e30, t2=1e30;
    // bool val1 = doesIntersectBound(intersection.ray, t1, tree[2*node +1].left, tree[2*node +1].right);
    // bool val2 = doesIntersectBound(intersection.ray, t2, tree[2*node +2].left, tree[2*node +2].right);

    // if (val1 && val2){
    //     bool BVcollide = doesBVCollide(tree[2*node +1].left, tree[2*node +1].right,
    //                                    tree[2*node +2].left, tree[2*node +2].right);
    //     if (!BVcollide) {
    //         if (t1<=t2) {
    //             if (intersectTree(2*node + 1, intersection)) return true;
    //             if (intersectTree(2*node + 2, intersection)) return true;
    //         }
    //         else { 
    //             if (intersectTree(2*node + 2, intersection)) return true;
    //             if (intersectTree(2*node + 1, intersection)) return true;
    //         }
    //     }
    //     else {
    //         bool checkBoth = false;
    //         if (intersectTree(2*node + 1, intersection)) checkBoth = true;
    //         if (intersectTree(2*node + 2, intersection)) checkBoth = true;
    //         return checkBoth;
    //     }
    // }
    // else if (val1) {
    //     if (intersectTree(2*node + 1, intersection)) return true;
    // }
    // else if (val2) {
    //     if (intersectTree(2*node + 2, intersection)) return true;
    // }

    // return false;
}

bool ShapeSet::doesBVCollide(const Vector &v1, const Vector &v2, const Vector &v3, const Vector &v4)
{
    float x1=v1.x, x2=v2.x, x3=v3.x, x4=v4.x;
    float y1=v1.y, y2=v2.y, y3=v3.y, y4=v4.y;
    float z1=v1.z, z2=v2.z, z3=v3.z, z4=v4.z;

    if (x1>x2) swap(x1, x2);
    if (x3>x4) swap(x3, x4);
    if (y1>y2) swap(y1, y2);
    if (y3>y4) swap(y3, y4);
    if (z1>z2) swap(z1, z2);
    if (z3>z4) swap(z3, z4);

    if ((x2<x4) && (x2<=x3)) return false;
    if ((y2<y4) && (y2<=y3)) return false;
    if ((z2<z4) && (z2<=z3)) return false;

    return true;
}

bool ShapeSet::doesIntersectBound(const Ray &ray, float &t, const Vector &v1, const Vector &v2)
{
    calls += 1;
    float t_minx = -1e30;
    float t_miny = -1e30;
    float t_minz = -1e30;
    float t_maxx = 1e30;
    float t_maxy = 1e30;
    float t_maxz = 1e30;

    // for x
    if ((ray.direction.x == 0.0f)) {
        if (((v1.x <= ray.origin.x) && (ray.origin.x <= v2.x)) ||
            ((v2.x <= ray.origin.x) && (ray.origin.x <= v1.x)))
        {
            // nothing
        }
        else return false;
    }
    else {
        t_minx = (v1.x - ray.origin.x) / ray.direction.x ;
        t_maxx = (v2.x - ray.origin.x) / ray.direction.x ;

        if (t_maxx < t_minx) std::swap(t_maxx, t_minx);
    }

    // for y
    if ((ray.direction.y == 0.0f)) {
        if (((v1.y <= ray.origin.y) && (ray.origin.y <= v2.y)) ||
            ((v2.y <= ray.origin.y) && (ray.origin.y <= v1.y)))
        {
            // nothing
        }
        else return false;
    }
    else {
        t_miny = (v1.y - ray.origin.y) / ray.direction.y ;
        t_maxy = (v2.y - ray.origin.y) / ray.direction.y ;

        if (t_maxy < t_miny) std::swap(t_maxy, t_miny);
    }

    // for z
    if ((ray.direction.z == 0.0f)) {
        if (((v1.z <= ray.origin.z) && (ray.origin.z <= v2.z)) ||
            ((v2.z <= ray.origin.z) && (ray.origin.z <= v1.z)))
        {
            // nothing
        }
        else return false;
    }
    else {
        t_minz = (v1.z - ray.origin.z) / ray.direction.z ;
        t_maxz = (v2.z - ray.origin.z) / ray.direction.z ;

        if (t_maxz < t_minz) std::swap(t_maxz, t_minz);
    }

    if (min(t_maxx, min(t_maxy, t_maxz)) < max(t_minx, max(t_miny, t_minz))) return false;

    t = max(t_minx, max(t_miny, t_minz));
    if (min(t_maxx, min(t_maxy, t_maxz)) < 0.0f) return false;

    return true;
}

int ShapeSet::setBounds(int node, int low, int high)
{
    float minx = (1e30);
    float miny = (1e30);
    float minz = (1e30);
    float maxx = -(1e30);
    float maxy = -(1e30);
    float maxz = -(1e30);

    for (int i=low; i<high+1; i++) {
        minx = min(minx, shapes[i]->maxAlong(Vector(-1, 0, 0)).x);
        miny = min(miny, shapes[i]->maxAlong(Vector(0, -1, 0)).y);
        minz = min(minz, shapes[i]->maxAlong(Vector(0, 0, -1)).z);
        maxx = max(maxx, shapes[i]->maxAlong(Vector(1, 0, 0)).x);
        maxy = max(maxy, shapes[i]->maxAlong(Vector(0, 1, 0)).y);
        maxz = max(maxz, shapes[i]->maxAlong(Vector(0, 0, 1)).z);
    }

    int axis;
    float val = -1e30;

    if (maxx - minx > val) {
        val = maxx - minx;
        axis = 0;
    }

    if (maxy - miny > val){
        val = maxy - miny;
        axis = 1;
    }

    if (maxz - minz > val) {
        val = maxz - minz;
        axis = 2;
    }
    tree[node].start = low;
    tree[node].end = high;
    tree[node].left = Vector(minx, miny, minz);
    tree[node].right = Vector(maxx, maxy, maxz);

    return axis;
}

void ShapeSet::buildTree(int node, int low, int high)
{
    if ((node >= 2*shapes.size() - 1) || high < low) return;
    int axis = setBounds(node, low, high);

    if (high - low + 1 < minSize) return;
    if (axis == 0) {
        std::sort(shapes.begin()+low, shapes.begin()+high+1, 
            [] (Shape *a, Shape *b) {
                if (a->getPoint().x != b->getPoint().x)
                    return a->getPoint().x < b->getPoint().x;
                else if(a->getPoint().y != b->getPoint().y)
                    return a->getPoint().y < b->getPoint().y;
                else
                    return a->getPoint().z < b->getPoint().z;
                });
    }
    else if (axis == 1) {
        std::sort(shapes.begin()+low, shapes.begin()+high+1, 
            [] (Shape *a, Shape *b) {
                if (a->getPoint().y != b->getPoint().y)
                    return a->getPoint().y < b->getPoint().y;
                else if(a->getPoint().x != b->getPoint().x)
                    return a->getPoint().x < b->getPoint().x;
                else
                    return a->getPoint().z < b->getPoint().z;
                });
    }
    else {
        std::sort(shapes.begin()+low, shapes.begin()+high+1, 
            [] (Shape *a, Shape *b) {
                if (a->getPoint().z != b->getPoint().z)
                    return a->getPoint().z < b->getPoint().z;
                else if(a->getPoint().x != b->getPoint().x)
                    return a->getPoint().x < b->getPoint().x;
                else
                    return a->getPoint().y < b->getPoint().y;
                });
    }

    int index = (low + high) / 2;
    buildTree(2*node + 1, low, index);
    buildTree(2*node + 2, index+1, high);
}

Vector ShapeSet::maxAlong(const Vector &axis)
{
    return Vector();
}

void ShapeSet::printTree()
{
    for (int i=0; i<10000; i++) {
        if (tree[i].start == -1) break;
        cout<<i<<" . {\n";
        cout<<""<<tree[i];
        cout<<"}\n";
    }

    for (int i=0; i<shapes.size(); i++) {
        Point p = shapes[i]->getPoint();
        cout<<i<<". {\n";
        cout<<p<<"\n}\n";
    }
}

Plane::Plane(const Point &position, const Vector &normal) : position(position),
                                                            normal(normal),
                                                            matarial()
{
}

Plane::~Plane()
{
}

bool Plane::intersect(Intersection &intersection)
{
    // checking for intersection
    float dn = dot(intersection.ray.direction, normal);

    // direction dot normal must not be 0.0f
    if (dn == 0.0f)
        return false;

    // find point of intersection
    float t = dot(position - intersection.ray.origin, normal) / dn;

    // t must be in acceptable range of t_min and t_max of intersection
    if (t <= RAY_T_MIN || t > intersection.t)
        return false;

    Point p = intersection.ray.calculate(t);
    if ((p - position).length2() > sqr(radius)) {
        return false;
    }

    // updating intersection
    intersection.t = t;
    intersection.pShape = this;

    return true;
}

bool Plane::doesIntersect(const Ray &ray)
{
    // checking for intersection
    float dn = dot(ray.direction, normal);

    // direction dot normal must not be 0.0f
    if (dn == 0.0f)
        return false;

    // find point of intersection
    float t = dot(position - ray.origin, normal) / dn;

    // t must be in acceptable range of t_min and t_max of ray
    if (t <= RAY_T_MIN || t >= ray.tMax)
        return false;

    Point p = ray.calculate(t);
    if ((p - position).length2() > sqr(radius)) return false;

    return true;
}

Point Plane::getPoint()
{
    return position;
}

Vector Plane::getNormal(const Point &point)
{
    if (dot(normal, point - position) > 0.0f)
        return normal;
    return -normal;
}

Light Plane::light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes)
{
    if (depth > maxDepth)
        return sky;

    return calculateLight(point, lastPoint, income, getNormal(lastPoint), matarial, shapes, depth, inside);
}

void Plane::setMatarial(const Matarial &matarial)
{
    this->matarial = matarial;
}

Matarial Plane::getMatarial()
{
    return matarial;
}

Vector Plane::maxAlong(const Vector &axis)
{
    Vector v(axis);
    v.normalize();
    return Vector(radius * (v - (normal * dot(v, normal))) + position);
}

void Plane::setRadius(float r)
{
    radius = r;
}

float Plane::getRadius()
{
    return radius;
}

Sphere::Sphere(const Point &center, float radius) : center(center),
                                                    radius(radius),
                                                    matarial()
{
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(Intersection &intersection)
{
    // shifting the ray to origin
    Ray shifRay = intersection.ray;
    shifRay.origin -= center;

    // solving for intersections
    float a = shifRay.direction.length2();
    float b = 2 * dot(shifRay.direction, shifRay.origin);
    float c = shifRay.origin.length2() - sqr(radius);

    float discriminant = sqr(b) - 4 * a * c;

    // checking for intersection
    if (discriminant < 0.0f)
        return false;

    float t1 = ((-b - sqrt(discriminant)) / (2 * a));
    float t2 = ((-b + sqrt(discriminant)) / (2 * a));

    if (t1 > RAY_T_MIN && t1 < intersection.t)
    {
        intersection.t = t1;
    }
    else if (t2 > RAY_T_MIN && t2 < intersection.t)
    {
        intersection.t = t2;
    }
    else
    {
        // Neither is valid
        return false;
    }

    // Finish populating intersection
    intersection.pShape = this;

    return true;
}

bool Sphere::doesIntersect(const Ray &ray)
{
    // shifting the ray to origin
    Ray shifRay = ray;
    shifRay.origin -= center;

    // solving for intersections
    float a = shifRay.direction.length2();
    float b = 2 * dot(shifRay.direction, shifRay.origin);
    float c = shifRay.origin.length2() - sqr(radius);

    float discriminant = sqr(b) - 4 * a * c;

    // checking for intersection
    if (discriminant < 0.0f)
        return false;

    float t1 = (-b - sqrt(discriminant) / (2 * a));
    float t2 = (-b + sqrt(discriminant) / (2 * a));

    // First check if close intersection is valid
    if (t1 > RAY_T_MIN && t1 < ray.tMax)
    {
        return true;
    }

    if (t2 > RAY_T_MIN && t2 < ray.tMax)
    {
        return true;
    }
    return false;
}

Point Sphere::getPoint()
{
    return center;
}

Vector Sphere::getNormal(const Point &point)
{
    return (point - center).normalized();
}

Light Sphere::light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes)
{

    if (depth > maxDepth)
        return sky;

    return calculateLight(point, lastPoint, income, getNormal(point), matarial, shapes, depth, inside);
}

void Sphere::setMatarial(const Matarial &matarial)
{
    this->matarial = matarial;
}

Matarial Sphere::getMatarial()
{
    return matarial;
}

Vector Sphere::maxAlong(const Vector &axis)
{
    Vector v(axis);
    return (radius * v.normalized() + center);
}

Triangle::Triangle(const Point& point1, const Point& point2, const Point& point3)
                                                                      :point1(point1),
                                                                      point2(point2),
                                                                      point3(point3),
                                                                      matarial()
{
    normal = cross((point1 - point2), (point3 - point2)).normalized();
}

Triangle::~Triangle()
{
}

bool Triangle::PointInTriangle (const Point &pt)
{
    float d1, d2, d3;

    d1 = dot(normal, cross(pt - point2, point1 - point2));
    d2 = dot(normal, cross(pt - point1, point3 - point1));
    d3 = dot(normal, cross(pt - point3, point2 - point3));

    return (d1>=0 && d2 >=0 && d3>=0) || (d1<=0 && d2<=0 && d3<=0);
}

bool Triangle::intersect(Intersection &intersection)
{
    // checking for intersection
    float dn = dot(intersection.ray.direction, normal);

    // direction dot normal must not be 0.0f
    if (dn == 0.0f)
        return false;

    // find point of intersection
    float t = dot(point1 - intersection.ray.origin, normal) / dn;

    // t must be in acceptable range of t_min and t_max of intersection
    if (t <= RAY_T_MIN || t > intersection.t)
        return false;

    Point pos = intersection.ray.calculate(t);
    if (!PointInTriangle(pos))
        return false;

    // updating intersection
    intersection.t = t;
    intersection.pShape = this;

    return true;
}

bool Triangle::doesIntersect(const Ray &ray)
{
    // checking for intersection
    float dn = dot(ray.direction, normal);

    // direction dot normal must not be 0.0f
    if (dn == 0.0f)
        return false;

    // find point of intersection
    float t = dot(point1 - ray.origin, normal) / dn;

    // t must be in acceptable range of t_min and t_max of ray
    if (t <= RAY_T_MIN || t >= ray.tMax)
        return false;

    Point pos = ray.calculate(t);
    if (!PointInTriangle(pos))
        return false;

    return true;
}

Point Triangle::getPoint()
{
    return (point1 + point2 + point3)/3;
}

Light Triangle::light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes)
{
    if (depth > maxDepth)
        return sky;

    return calculateLight(point, lastPoint, income, getNormal(lastPoint), matarial, shapes, depth, inside);
}

Vector Triangle::getNormal(const Point &point)
{
    if (dot(normal, point - point1) > 0.0f)
        return normal;
    return -normal;
}

void Triangle::setMatarial(const Matarial &matarial)
{
    this->matarial = matarial;
}

Matarial Triangle::getMatarial()
{
    return matarial;
}

Vector Triangle::maxAlong(const Vector &axis)
{
    Vector ans;
    float val = -1e30;
    if (dot(axis, point1)>val) {
        val = dot(axis, point1);
        ans = Vector(point1);
    }
    if (dot(axis, point2) > val) {
        val = dot(axis, point2);
        ans = Vector(point2);
    }
    if (dot(axis, point3) > val) {
        val = dot(axis, point3);
        ans = Vector(point3);
    }
    return ans;
}



Polygon::Polygon(const vector<Point>& points)
                              :points(points),
                              matarial()
{
    normal = cross((points[0] - points[1]), (points[2] - points[1])).normalized();
}

Polygon::~Polygon()
{
}

bool Polygon::PointInPolygon(const Point &pt)
{
    float d = 0.0f, temp;
    int n = points.size();
    for (int i=0; i<n; i++){
        Point v1 = points[(i%n)];
        Point v2 = points[((i+1)%n)];
        temp = dot(normal, cross(pt - v1, v2 - v1));
        if ((temp >= 0.0f && d>=0.0f) || (temp <= 0.0f && d<= 0.0f))
            d = temp;
        else
            return false;
    }
    return true;
}

bool Polygon::intersect(Intersection &intersection)
{
    // checking for intersection
    float dn = dot(intersection.ray.direction, normal);

    // direction dot normal must not be 0.0f
    if (dn == 0.0f)
        return false;

    // find point of intersection
    float t = dot(points[0] - intersection.ray.origin, normal) / dn;

    // t must be in acceptable range of t_min and t_max of intersection
    if (t <= RAY_T_MIN || t > intersection.t)
        return false;

    Point pos = intersection.ray.calculate(t);
    if (!PointInPolygon(pos))
        return false;

    // updating intersection
    intersection.t = t;
    intersection.pShape = this;

    return true;
}

bool Polygon::doesIntersect(const Ray &ray)
{
    // checking for intersection
    float dn = dot(ray.direction, normal);

    // direction dot normal must not be 0.0f
    if (dn == 0.0f)
        return false;

    // find point of intersection
    float t = dot(points[0] - ray.origin, normal) / dn;

    // t must be in acceptable range of t_min and t_max of ray
    if (t <= RAY_T_MIN || t >= ray.tMax)
        return false;

    Point pos = ray.calculate(t);
    if (!PointInPolygon(pos))
        return false;

    return true;
}

Point Polygon::getPoint()
{
    Point ans = Point();
    for (int i=0; i<points.size(); i++){
        Point p = points[i];
        ans = ans + p;
    }
    ans /= points.size();
    return ans;
}

Light Polygon::light(const Point &point, const Point &lastPoint, Vector &income, int depth, bool inside, ShapeSet *shapes)
{
    if (depth > maxDepth)
        return sky;

    return calculateLight(point, lastPoint, income, getNormal(lastPoint), matarial, shapes, depth, inside);
}

Vector Polygon::getNormal(const Point &point)
{
    if (dot(normal, point - points[0]) > 0.0f)
        return normal;
    return -normal;
}

void Polygon::setMatarial(const Matarial &matarial)
{
    this->matarial = matarial;
}

Matarial Polygon::getMatarial()
{
    return matarial;
}

Vector Polygon::maxAlong(const Vector &axis)
{
    int ans;
    float val = -1e30;
    
    for (int i=0; i<points.size(); i++) {
        Vector v(points[i]);
        float res = dot(axis, v);
        if (res > val) {
            val = res;
            ans = i; 
        }
    }
    return Vector(points[ans]);
}