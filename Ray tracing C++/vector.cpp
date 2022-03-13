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

Vector::Vector(const glm::vec3 &v)
{
	Vector(v.x, v.y, v.z);
}

Vector::~Vector()
{
}

inline float Vector::length2()
{
	return glm::length2(glm::vec3(x, y, z));
	// return sqr(x) + sqr(y) + sqr(z);
}

inline float Vector::length()
{
	return glm::length(glm::vec3(x, y, z));
	//return std::sqrt(length2());
}

float Vector::normalize()
{
	float l = length();
	glm::vec3 v(glm::normalize(glm::vec3(x, y, z)));
	x = v.x;
	y = v.y;
	z = v.z;

	// if (l == 0.0f) {
	// 	// cout<<"Divide by zero! returning zero vector";
	// 	*this = Vector();
	// }
	// *this /= l;
	// Danish

	return l;
}

Vector Vector::normalized()
{
	return Vector(glm::normalize(glm::vec3(x, y, z)));
}

Vector Vector::vectorize(const glm::vec3 &v)
{
	return Vector(v);
}


float dot(Vector v1, Vector v2)
{
	return glm::dot(glm::vec3(v1.x, v1.y, v1.z), glm::vec3(v2.x, v2.y, v2.z));
	// return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

Vector cross(Vector v1, Vector v2)
{
	return Vector(glm::cross(glm::vec3(v1.x, v1.y, v1.z), glm::vec3(v2.x, v2.y, v2.z)));
	// return Vector(v1.y * v2.z - v1.z * v2.y,
	// 			  v1.z * v2.x - v1.x * v2.z,
	// 			  v1.x * v2.y - v1.y * v2.x);
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