#include "stdafx.h"
#include "Plane.h"
#include "BasicGeometry.h"


Plane::Plane(Vect3d& p, Vect3d& u, Vect3d& v, bool arePoints)
{	
	set(p, u, v, arePoints);
}

Plane::Plane(const Plane& plane) : _a(plane._a), _b(plane._b), _c(plane._c)
{
}

Plane::~Plane()
{
}

void Plane::set(const Vect3d& p, const Vect3d& u, const Vect3d& v, bool arePoints)
{
	if (!arePoints)			// Vectors: pi = p + u * lambda + v * mu 
	{
		_a = p;
		_b = u.add(_a);
		_c = v.add(_a);
	}
	else
	{						// 3 points in the plane
		_a = p;
		_b = u;
		_c = v;
	}
}

void Plane::setFromEmplicit(Vect3d n, float d)
{
	Vect3d a(-d / n.getX(), 0.0, 0.0);
	Vect3d b(0.0, -d / n.getY(), 0.0);
	Vect3d c(0.0, 0.0, -d / n.getZ());

	_a = a;
	_b = b;
	_c = c;
}

bool Plane::coplanar(Vect3d& point)
{
	return (BasicGeometry::equal(distance(point), 0.0));
}

double Plane::distance(Vect3d& point)
{
	Vect3d n = getNormal();
	double t =	-(n.dot(point) + getD()) / n.dot(n);
	double dist = n.scalarMul(t).module();
	return dist;
}

double Plane::getA()
{
	return (BasicGeometry::determinant2x2(	_c.getZ() - _a.getZ(), _c.getY() - _a.getY(), 
											_b.getZ() - _a.getZ(), _b.getY() - _a.getY()));
}

double Plane::getB()
{
	return (BasicGeometry::determinant2x2(	_c.getX() - _a.getX(), _c.getZ() - _a.getZ(), 
											_b.getX() - _a.getX(), _b.getZ() - _a.getZ()));
}

double Plane::getC()
{
	return (BasicGeometry::determinant2x2(	_c.getY() - _a.getY(), _c.getX() - _a.getX(), 
											_b.getY() - _a.getY(), _b.getX() - _a.getX()));
}

Vect3d Plane::getNormal()
{
	Vect3d v1 = _a.sub(_b);
	Vect3d v2 = _a.sub(_c);
	
	return v1.xProduct(v2);
}

bool Plane::intersect(Plane& plane, Line3d& line)
{
	Vect3d n1 = getNormal();
	Vect3d n2 = plane.getNormal();
	Vect3d n3 = getNormal().xProduct(n2);

	double det = BasicGeometry::determinant3x3(	n1.getX(), n1.getY(), n1.getZ(), 
												n2.getX(), n2.getY(), n2.getZ(),
												n3.getX(), n3.getY(), n3.getZ());

	if (BasicGeometry::equal(det, 0.0))
		return false;

	Vect3d lPoint;

	lPoint.setX(((plane.getD() * BasicGeometry::determinant2x2(n1.getY(), n1.getZ(), n3.getY(), n3.getZ())) -
		(getD() * BasicGeometry::determinant2x2(n2.getY(), n2.getZ(), n3.getY(), n3.getZ()))) / det);

	lPoint.setY(((plane.getD() * BasicGeometry::determinant2x2(n3.getX(), n3.getZ(), n1.getX(), n1.getZ())) -
		(getD() * BasicGeometry::determinant2x2(n3.getX(), n3.getZ(), n2.getX(), n2.getZ()))) / det);

	lPoint.setZ(((plane.getD() * BasicGeometry::determinant2x2(n1.getX(), n1.getY(), n3.getX(), n3.getY())) -
		(getD() * BasicGeometry::determinant2x2(n2.getX(), n2.getY(), n3.getX(), n3.getY()))) / det);

	line.setOrigin(lPoint);
	line.setDestination(lPoint.add(n3));

	return true;
}

bool Plane::intersect(Line3d& line, Vect3d& point)
{
	Vect3d lOrig = line.getOrigin();
	Vect3d n = getNormal();
	Vect3d v = line.getDestination().sub(lOrig);
	double denominator = n.dot(v);

	if (BasicGeometry::equal(denominator, 0.0))
		return false;

	double t = -(n.dot(lOrig) + getD()) / denominator;
	point = line.getPoint(t);

	return true;
}

bool Plane::intersect(Plane& pa, Plane& pb, Vect3d& pinter)
{
	//XXXX
	return true;
}

Vect3d Plane::reflectedPoint(Vect3d& v)
{
	Vect3d n = getNormal();
	double t = (-2.0 * (n.dot(v) + getD())) / n.dot(n);

	return n.scalarMul(t).add(v);
}

Plane& Plane::operator=(const Plane& plane)
{
	if (this != &plane)
	{
		_a = plane._a;
		_b = plane._b;
		_c = plane._c;
	}

	return *this;
}

std::vector<Vect3d> Plane::getDrawingPoints(float t)
{
	std::vector<Vect3d> points(7);
	Vect3d AB = _b.sub(_a);
	Vect3d AC = _c.sub(_a);
	Vect3d BC = _c.sub(_b);

	//ab
	points.push_back((AB.scalarMul(t)).add(_b));

	//ac
	points.push_back((AC.scalarMul(t)).add(_c));

	//bc
	points.push_back((BC.scalarMul(t)).add(_c));

	//ba
	points.push_back((AB.scalarMul(-t)).add(_a));

	//ca
	points.push_back((AC.scalarMul(-t)).add(_a));

	//cb
	points.push_back((BC.scalarMul(-t)).add(_b));

	//ab
	points.push_back((AB.scalarMul(t)).add(_b));

	return points;
}

std::ostream& operator<<(std::ostream& os, const Plane& plane)
{
	os << "Plane -> a: " << plane._a << ", b: " << plane._b << ", c: " << plane._c;
	return os;
}