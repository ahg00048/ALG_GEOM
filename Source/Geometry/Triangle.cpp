#include "stdafx.h"
#include "Circle.h"
#include "Triangle.h"

// Public methods

Triangle::Triangle(Vect2d &aa, Vect2d &bb, Vect2d &cc): _a(aa), _b(bb), _c(cc)
{
}

Triangle::Triangle(const Triangle &tt): _a(tt._a), _b(tt._b), _c(tt._c) 
{
} 

Circle Triangle::getInscribed() const {
	double sP = _a.distance(_b) + _b.distance(_c) + _a.distance(_c);
	double radius = (_a.triangleArea2(_b, _c) / sP);
	double Xp = (_a.getX() * _b.distance(_c) + _c.getX() * _b.distance(_a) + _b.getX() * _a.distance(_c)) / (2 * sP);
	double Yp = (_a.getY() * _b.distance(_c) + _c.getY() * _b.distance(_a) + _b.getY() * _a.distance(_c)) / (2 * sP);
	return Circle(Point(Xp, Yp), radius);
}

Circle Triangle::getCirumscribed() const {
	double radius = (_a.distance(_b) * _b.distance(_c) * _c.distance(_a)) / (2 * _a.triangleArea2(_b, _c));
	double Xp = _a.getX() * (BasicGeometry::determinant2x2(_c.getY() - _a.getY(), _a.distance(_c), _b.getY() - _a.getY(), _a.distance(_b)) / (2 * _a.triangleArea2(_b, _c)));
	double Yp = _a.getY() * (BasicGeometry::determinant2x2(_a.distance(_c), _c.getX() - _a.getX(), _a.distance(_b), _b.getX() - _a.getX()) / (2 * _a.triangleArea2(_b, _c)));
	return Circle(Point(Xp, Yp), radius);
}
