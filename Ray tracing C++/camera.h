#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"

#include "vector.h"
#include "ray.h"

class Camera
{
public:
	virtual ~Camera() {}

	virtual Ray makeRay(Vector2 point) const = 0;
	virtual Ray getRay(int x, int y, int pWidth, int pHeight) const = 0;
	virtual Ray getRays(float x, float y, int pWidth, int pHeight) const = 0;
};

class PerspectiveCamera : public Camera
{
protected:
	Point origin;
	Vector forward;
	Vector up;
	Vector right;

	float h, w;

public:
	PerspectiveCamera(Point origin, Vector target,
					  Vector upguide, float fov, float aspectRatio);

	virtual Ray makeRay(Vector2 point) const;
	virtual Ray getRay(int x, int y, int pHeight, int pWidth) const;
	virtual Ray getRays(float x, float y, int pWidth, int pHeight) const;

};

#endif // CAMERA_H