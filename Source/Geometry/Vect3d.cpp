#include "stdafx.h"
#include "Vect3d.h"

#include "Triangle3d.h"
#include "BasicGeometry.h"

Vect3d::Vect3d()
{
	setVert(FLT_MAX, FLT_MAX, FLT_MAX);
}

Vect3d::Vect3d(vec3 vec) 
{
	_value[X] = vec.x;
	_value[Y] = vec.y;
	_value[Z] = vec.z;
}

Vect3d::Vect3d(double x, double y, double z)
{
	_value[X] = x;
	_value[Y] = y;
	_value[Z] = z;
}

Vect3d::Vect3d(const Vect3d& vector)
{
	_value[X] = vector._value[X];
	_value[Y] = vector._value[Y];
	_value[Z] = vector._value[Z];
}

Vect3d::~Vect3d()
{
}

Vect3d Vect3d::add(Vect3d& b) const
{
	return Vect3d(getX() + b.getX(), getY() + b.getY(), getZ() + b.getZ());
}

bool Vect3d::collinear(Vect3d& a, Vect3d& b)
{
	Triangle3d tr(a, b, *this);

	return BasicGeometry::equal(tr.area(), BasicGeometry::EPSILON);
}

double Vect3d::distance(Vect3d& p) const
{
	return sub(p).module();
}

double Vect3d::dot(Vect3d& v) const
{
	return (getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ());
}

std::vector<double> Vect3d::getVert()
{
	return std::vector<double> {getX(), getY(), getZ()};
}

double Vect3d::getX() const
{
	return _value[X];
}

double Vect3d::getY() const
{
	return _value[Y];
}

double Vect3d::getZ() const
{
	return _value[Z];
}

double Vect3d::module() const
{
	return sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
}

Vect3d& Vect3d::operator=(const Vect3d& vector)
{
	_value[X] = vector._value[X];
	_value[Y] = vector._value[Y];
	_value[Z] = vector._value[Z];

	return *this;
}

bool Vect3d::operator==(const Vect3d& vector)
{
	return BasicGeometry::equal(getX(), vector._value[X]) && BasicGeometry::equal(getY(), vector._value[Y]) && BasicGeometry::equal(getZ(), vector._value[Z]);
}

bool Vect3d::operator!=(const Vect3d& vector)
{
	return !(this->operator==(vector));
}

std::ostream& operator<<(std::ostream& os, const Vect3d& vec)
{
	os << "Coordinate X: " << std::to_string(vec.X) << ", coordinate Y: " << std::to_string(vec.Y) << ", coordinate Z: " << std::to_string(vec.Z);
	return os;
}

Vect3d Vect3d::scalarMul(double value) const
{
	return Vect3d(getX() * value, getY() * value, getZ() * value);
}

void Vect3d::setX(double x)
{
	_value[X] = x;
}

void Vect3d::setY(double y)
{
	_value[Y] = y;
}

void Vect3d::setZ(double z)
{
	_value[Z] = z;
}

void Vect3d::setVert(double x, double y, double z)
{
	_value[X] = x;
	_value[Y] = y;
	_value[Z] = z;
}

Vect3d Vect3d::sub(Vect3d& b) const
{
	return Vect3d(getX() - b.getX(), getY() - b.getY(), getZ() - b.getZ());
}

Vect3d Vect3d::xProduct(Vect3d& b) const
{
	return Vect3d(getY() * b.getZ() - getZ() * b.getY(), getZ() * b.getX() - getX() * b.getZ(), getX() * b.getY() - getY() * b.getX());
}

Vect3d Vect3d::normalize()
{
	double mod = module();

	return Vect3d(_value[X] / mod, _value[Y] / mod, _value[Z] / mod);
}

void Vect3d::getPlane(Vect3d& v, Vect3d& n, float& d) const
{
	n.setX(v.getX() - getX());
	n.setY(v.getY() - getY());
	n.setZ(v.getZ() - getZ());

	d = (pow(v.getX(), 2) - pow(getX(), 2) + pow(v.getY(), 2) - pow(getY(), 2) + pow(v.getZ(), 2) - pow(getZ(), 2))
		/ 2.0f;
}

Vect3d::operator vec3() const
{
	return vec3(_value[X], _value[Y], _value[Z]);
}