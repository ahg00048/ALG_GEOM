#include "stdafx.h"
#include "Line.h"

#include "RayLine.h"
#include "SegmentLine.h"


Line::Line(const Point & a, const Point & b) : SegmentLine(a, b)
{
}

Line::Line(const Line & s) : SegmentLine(s)
{
}

Line::~Line()
{
}

double Line::distToPoint(Vect2d& v)
{
	Vect2d d(_dest.getX() - _orig.getX(), _dest.getY() - _orig.getY());
	Vect2d pa = v - _orig;

	float t = (d.dot(pa) /
		d.getModule());

	return (v - (d.scalarMult(t) + _orig)).getModule();
}

bool Line::intersects(const Line& line, Vect2d& intersection)
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = line.getA();
	Vect2d lineDest = line.getB();

	if (SegmentLine::intersects(lineOrig, lineDest, t, s))
	{
		intersection = getPoint(s);
		return true;
	}

	return false;
}

bool Line::intersects(const RayLine& rayline, Vect2d& intersection)
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = rayline.getA();
	Vect2d lineDest = rayline.getB();

	if (SegmentLine::intersects(lineOrig, lineDest, t, s) && 
					0 <= t)
	{
		intersection = getPoint(s);
		return true;
	}

	return false;
}

bool Line::intersects(const SegmentLine& segment, Vect2d& intersection)
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = segment.getA();
	Vect2d lineDest = segment.getB();

	if (SegmentLine::intersects(lineOrig, lineDest, t, s) && 
					(0 <= t && t <= 1))
	{
		intersection = getPoint(s);
		return true;
	}

	return false;
}

bool Line::incorrectSegmentIntersection(SegmentLine& l)
{
	return false;
}

Line & Line::operator=(const Line & line)
{
	if (this != &line)
	{
		SegmentLine::operator=(line);
	}

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Line& line)
{
	os << "Line --->\n\tPoint A: " << line._orig << "\n\tPoint B: " << line._dest << "\n";

	return os;
}

bool Line::segmentIntersection(SegmentLine& l)
{
	// XXXX
	return false;
}