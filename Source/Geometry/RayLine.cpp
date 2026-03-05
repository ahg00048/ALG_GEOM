#include "stdafx.h"
#include "RayLine.h"

#include "Line.h"

// Public methods

RayLine::RayLine(const Point& a, const Point& b) : SegmentLine(a, b)
{
}

RayLine::RayLine(const RayLine& ray) : SegmentLine(ray)
{
}

RayLine::~RayLine()
{
}

float RayLine::distToPoint(Vect2d& vector)
{
	float distance = 0.0f;
	Vect2d d(_dest.getX() - _orig.getX(), _dest.getY() - _orig.getY());
	Vect2d pa = vector - _orig;
	float t = (d.dot(pa) /
				d.getModule());
	
	return (BasicGeometry::lessOrEqual(t, 0)) ? pa.getModule() : (vector - (d.scalarMult(t) + _orig)).getModule();
}

bool RayLine::incorrectSegmentIntersection(SegmentLine& segment)
{
	return false;
}

RayLine & RayLine::operator=(const RayLine & rayline)
{
	if (this != &rayline)
	{
		SegmentLine::operator=(rayline);
	}

	return *this;
}

std::ostream& operator<<(std::ostream& os, const RayLine& ray)
{
	os << "Point A: " << ray._orig << ", Point B: " << ray._dest << "\n";

	return os;
}

bool RayLine::intersects(const RayLine& r, Vect2d& v)
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = r.getA();
	Vect2d lineDest = r.getB();

	if (SegmentLine::intersects(lineOrig, lineDest, t, s) &&
		BasicGeometry::lessOrEqual(0, s) &&
		BasicGeometry::lessOrEqual(0 , t))
	{
		v = getPoint(s);
		return true;
	}

	return false;
}

bool RayLine::intersects(const Line& l, Vect2d& v)
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = l.getA();
	Vect2d lineDest = l.getB();

	if (SegmentLine::intersects(lineOrig, lineDest, t, s) &&
		BasicGeometry::lessOrEqual(0, s))
	{
		v = getPoint(s);
		return true;
	}

	return false;
}

bool RayLine::intersects(const SegmentLine& sg, Vect2d& v) 
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = sg.getA();
	Vect2d lineDest = sg.getB();

	if (SegmentLine::intersects(lineOrig, lineDest, t, s) &&
		BasicGeometry::lessOrEqual(0, s) &&
		(BasicGeometry::lessOrEqual(0, t) && BasicGeometry::lessOrEqual(t, 1)))
	{
		v = getPoint(s);
		return true;
	}

	return false;
}