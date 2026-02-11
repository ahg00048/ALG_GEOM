#include "stdafx.h"
#include "SegmentLine.h"

// Public methods

SegmentLine::SegmentLine()
{
}

SegmentLine::SegmentLine(const Point& a, const Point& b) : _orig(a), _dest(b)
{
}

SegmentLine::SegmentLine(const SegmentLine& segment) : _orig(segment._orig), _dest(segment._dest)
{
}

SegmentLine::SegmentLine(double ax, double ay, double bx, double by) : _orig(ax, ay), _dest(bx, by)
{
}

SegmentLine::~SegmentLine()
{
}

SegmentLine & SegmentLine::operator=(const SegmentLine & segment)
{
	if (this != &segment)
	{
		this->_orig = segment._orig;
		this->_dest = segment._dest;
	}

	return *this;
}

bool SegmentLine::segmentIntersection(SegmentLine& l)
{
	Point lOrig = l.getA();
	Point lDest = l.getB();
	return _orig.left(lOrig, lDest) ^ _dest.left(lOrig, lDest) && lOrig.left(_orig, _dest) ^ lDest.left(_orig, _dest);
}

double SegmentLine::getC()
{
	double m = slope(); 

	return (m == INFINITY) ? INFINITY : _orig.getY() - m * _orig.getX();
}

bool SegmentLine::distinct(SegmentLine & segment)
{
	return !equal(segment);
}

float SegmentLine::distPointSegment(Vect2d& vector)
{
	return 0.0f;
}

bool SegmentLine::equal(SegmentLine & segment)
{
	return (_orig.equal(segment._orig) && _dest.equal(segment._dest)) || (_orig.equal(segment._dest) && _dest.equal(segment._orig));
}

Point SegmentLine::getPoint(double t)
{
	return Point(_orig.getX() + ((_dest.getX() - _orig.getX()) * t), 
		_orig.getY() + ((_dest.getY() - _orig.getY()) * t));
}

bool SegmentLine::impSegmentIntersection(SegmentLine& segment)
{
    return _orig.colinear(segment._dest, segment._orig) || segment._orig.colinear(_dest, _orig) || 
		_dest.colinear(segment._dest, segment._orig) || segment._dest.colinear(_dest, _orig);
}

bool SegmentLine::isHorizontal()
{
	return BasicGeometry::equal(_orig.getY(), _dest.getY());
}

bool SegmentLine::isVertical()
{
	return BasicGeometry::equal(_orig.getX(), _dest.getX());
}

double SegmentLine::slope()
{
    return (BasicGeometry::equal(_dest.getX(), _orig.getX())) ? INFINITY : (_dest.getY() - _dest.getY())/(_dest.getX() - _orig.getX());
}

// Protected methods

std::ostream& operator<<(std::ostream& os, const SegmentLine& segment)
{
	os << "Point A: " << segment._orig << ", Point B: " << segment._dest << "\n";

	return os;
}

// Protected methods

float SegmentLine::getDistanceT0(Vect2d& point)
{
	return 0.0f;
}