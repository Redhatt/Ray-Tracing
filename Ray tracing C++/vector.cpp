#include "globals.h"

#include "vector.h"

using namespace std;

Vector::Vector()
	: x(0.0),
	  y(1.0),
	  z(0.0)
{
}

Vector::Vector(const Vector &v)
	: x(v.x),
	  y(v.y),
	  z(v.z)
{
}

Vector::Vector(float x, float y, float z)
	: x(x),
	  y(y),
	  z(z)
{
}

Vector::Vector(float f)
	: x(f),
	  y(f),
	  z(f)
{
}

Vector::~Vector()
{
}

inline float Vector::length2()
{
	return sqr(x) + sqr(y) + sqr(z);
}

inline float Vector::length()
{
	return std::sqrt(length2());
}

float Vector::normalize()
{
	float l = length();

	if (l == 0.0f) {
		// cout<<"Divide by zero! returning zero vector";
		*this = Vector();
	}
	*this /= l;

	return l;
}

Vector Vector::normalized()
{
	Vector v(*this);
	v.normalize();
	return v;
}


float dot(Vector v1, Vector v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vector cross(Vector v1, Vector v2)
{
	return Vector(v1.y * v2.z - v1.z * v2.y,
				  v1.z * v2.x - v1.x * v2.z,
				  v1.x * v2.y - v1.y * v2.x);
}

Vector randomVector(Vector direction, float angleDiv, float randomVal)
{
	Vector dirX = direction.normalized();
	Vector dirY = cross(dirX,
						Vector(dirX.x - randomVal,
							   dirX.y - 2 * randomVal,
							   dirX.z - 3 * randomVal))
					  .normalized();
	Vector dirZ = cross(dirX, dirY);
	return cos(randomVal * angleDiv) * dirX +
		   sin(randomVal * angleDiv) *
			   (cos(randomVal * 2 * PI) * dirY + sin(randomVal * 2 * PI) * dirZ);
}

Vector &Vector::operator=(const Vector &v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

Vector &Vector::operator+=(const Vector &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector &Vector::operator-=(const Vector &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector &Vector::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

Vector &Vector::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

Vector Vector::operator-() const
{
	return Vector(-x, -y, -z);
}

Vector2::Vector2()
	: u(0.0),
	  v(0.0)
{
}

Vector2::Vector2(const Vector2 &v)
	: u(v.u),
	  v(v.v)
{
}

Vector2::Vector2(float u, float v)
	: u(u),
	  v(v)
{
}

Vector2::Vector2(float f)
	: u(f),
	  v(f)
{
}

Vector2::~Vector2()
{
}

Vector2 &Vector2::operator=(const Vector2 &v)
{
	u = v.u;
	this->v = v.v;
	return *this;
}