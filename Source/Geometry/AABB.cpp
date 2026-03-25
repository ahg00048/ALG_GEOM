#include "stdafx.h"
#include "AABB.h"

AABB::AABB() : _min(-1.0f, -1.0f, -1.0f), _max(1.0f, 1.0f, 1.0f)
{
}

AABB::AABB(const Vect3d& min, const Vect3d& max) : _min(min), _max(max)
{
}

AABB::AABB(const AABB& aabb) : _min(aabb._min), _max(aabb._max)
{
}

AABB::AABB(const vec3& min, const vec3& max) : _max(max), _min(min)
{
}

AABB::~AABB()
{
}

Vect3d AABB::getCenter()
{
	return Vect3d(	(_min.getX() + _max.getX()) / 2.0, 
					(_min.getY() + _max.getY()) / 2.0, 
					(_min.getZ() + _max.getZ()) / 2.0);
}

Vect3d AABB::getExtent()
{
	return Vect3d(	(_max.getX() - _min.getX()) / 2.0,
					(_max.getY() - _min.getY()) / 2.0,
					(_max.getZ() - _min.getZ()) / 2.0);
}

AABB& AABB::operator=(const AABB& orig)
{
	_min = orig._min;
	_max = orig._max;

	return *this;
}

std::ostream& operator<<(std::ostream& os, const AABB& aabb)
{
	os << "Maximum: " << aabb._max << ", minimum: " << aabb._min;
	return os;
}

AABB AABB::dot(const mat4& matrix) const
{
	return AABB(matrix * vec4(vec3(_min), 1.0f), matrix * vec4(vec3(_max), 1.0f));
}

void AABB::update(const AABB& aabb)
{
	update(vec3(aabb.getMax()));
	update(vec3(aabb.getMin()));
}

void AABB::update(const vec3& point)
{
	if (point.x < _min.getX()) { _min.setX(point.x); }
	if (point.y < _min.getY()) { _min.setY(point.y); }
	if (point.z < _min.getZ()) { _min.setZ(point.z); }

	if (point.x > _max.getX()) { _max.setX(point.x); }
	if (point.y > _max.getY()) { _max.setY(point.y); }
	if (point.z > _max.getZ()) { _max.setZ(point.z); }
}

bool AABB::boxBox(const AABB& other) const
{
	// Si en cualquier eje están separados, el SAT retorna falso 
	if (_max.getX() < other._min.getX() || _min.getX() > other._max.getX()) return false;
	if (_max.getY() < other._min.getY() || _min.getY() > other._max.getY()) return false;
	if (_max.getZ() < other._min.getZ() || _min.getZ() > other._max.getZ()) return false; // Si no hay separación en ningún eje, hay colisión return true;

	return true;
}