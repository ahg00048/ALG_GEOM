#include "stdafx.h"
#include "Point.h"

// Public methods

Point::Point()
{
	_x = DEFAULT_VALUE;
	_y = DEFAULT_VALUE;
}

Point::Point(double x, double y, bool polar)
{
	if (!polar)
	{		
		_x = x; _y = y;
	} 
	else
	{
		//x: radius, y: angle
		_x = x * std::cos(y);
		_y = x * std::sin(y);
	}
}

Point::Point(const Point& point)
{
	_x = point._x;
	_y = point._y;
}

Point::~Point()
{
}

Point::PointClassification Point::classify(Point & p0, Point & p1)
{
	Point &p2 = *this;
	Point a = { p1._x - p0._x, p1._y - p0._y };
	Point b = { p2._x - p0._x, p2._y - p0._y };
	double sa = a._x * b._y - b._x * a._y;
	if (sa > 0.0) return PointClassification::LEFT;
	if (sa < 0.0) return PointClassification::RIGHT;
	if ((a._x * b._x < 0.0) || (a._y * b._y < 0.0)) return PointClassification::BACKWARD;
	if (a.getModule() < b.getModule()) return PointClassification::FORWARD;
	if (p0.equal(p2)) return PointClassification::ORIGIN;
	if (p1.equal(p2)) return PointClassification::DEST;
	return PointClassification::BETWEEN;
}

bool Point::colinear(Point& a, Point& b)
{
	PointClassification result = classify(a, b);
	return (result != PointClassification::LEFT) && (result != PointClassification::RIGHT);
}

double Point::distance(const Point & p) const
{
	return std::sqrt(std::pow(_x - p._x, 2) + std::pow(_y - p._y, 2));
}

double Point::getAlpha() const
{
	return std::atan(_y/_x); 
}

double Point::getModule() const
{
	return std::sqrt(std::pow(_x, 2) + std::pow(_y, 2));
}

bool Point::leftAbove(Point& a, Point& b)
{
	PointClassification result = classify(a, b);
	return (result == PointClassification::LEFT) || (result != PointClassification::RIGHT);
}

Point & Point::operator=(const Point & point)
{
	_x = point._x;
	_y = point._y;

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
	os << "Coordinate X: " << point._x << ", coordinate Y: " << point._y;

	return os;
}

bool Point::rightAbove(Point& a, Point& b)
{
	PointClassification result = classify(a, b);
	return (result == PointClassification::RIGHT) || (result != PointClassification::LEFT);
}

double Point::slope(Point & p) const
{
    return (BasicGeometry::equal(p._x, _x)) ? INFINITY : (p._y - _y) / (p._x - _x);
}

double Point::triangleArea2(const Point& a, const Point& b) const
{ 
	return _x * a._y - _y * a._x + a._x * b._y - a._y * b._x + b._x * _y - b._y * _x;
}
