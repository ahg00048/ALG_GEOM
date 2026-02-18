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
	const double A = _b.distance(_c);
	const double C = _b.distance(_a);
	const double B = _c.distance(_a);

	double sP = (C + A + B) / 2.0;
	double radius = fabs(_a.triangleArea2(_b, _c) / (2.0 * sP));
	double Xp = (_a.getX() * A + _c.getX() * C + _b.getX() * B) / (2.0 * sP);
	double Yp = (_a.getY() * A + _c.getY() * C + _b.getY() * B) / (2.0 * sP);
	
	return Circle(Point(Xp, Yp), radius);
}

Circle Triangle::getCirumscribed() const {
	const double A = _b.distance(_c);
	const double C = _b.distance(_a);
	const double B = _c.distance(_a);
	const double Xac = _c.getX() - _a.getX();
	const double Yac = _c.getY() - _a.getY();
	const double Xab = _b.getX() - _a.getX();
	const double Yab = _b.getY() - _a.getY();
	const double triangleArea4 = 2.0 * _a.triangleArea2(_b, _c);

	double radius = (C * A * B) / fabs(triangleArea4);
	double Xp = _a.getX() + ((BasicGeometry::determinant2x2(Yac, pow(B, 2), 
																pow(C, 2), Yab))
																/ triangleArea4);

	double Yp = _a.getY() + ((BasicGeometry::determinant2x2(pow(B, 2), Xac,
																Xab, pow(C, 2)))
																/ triangleArea4);

	return Circle(Point(Xp, Yp), radius);
}