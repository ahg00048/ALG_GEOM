#include "stdafx.h"
#include "SegmentLine.h"
#include "Line.h"
#include "RayLine.h"

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

bool SegmentLine::intersects(const Vect2d& p1, const Vect2d& p2, float& t, float& s) 
{
	double Xcd = p2.getX() - p1.getX();
	double Ycd = p2.getY() - p1.getY();

	double Xab = _dest.getX() - _orig.getX();
	double Yab = _dest.getY() - _orig.getY();

	double Xac = p1.getX() - _orig.getX();
	double Yac = p1.getY() - _orig.getY();

	double denominator = (Xcd * Yab - Xab * Ycd);

	if (BasicGeometry::equal(denominator, 0.0))
		return false;

	s = (Xcd * Yac - Xac * Ycd) /
		(Xcd * Yab - Xab * Ycd);

	t = (Xab * Yac - Xac * Yab) /
		(Xcd * Yab - Xab * Ycd);

	return true;
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

float SegmentLine::distToPointSeg(Vect2d& vector)
{
	float distance = 0.0f;
	Vect2d d(_dest.getX() - _orig.getX(), _dest.getY() - _orig.getY());
	Vect2d pa(vector.getX() - _orig.getX(), vector.getY() - _orig.getY());
	float t = (d.dot(pa) /
			   d.getModule());

	return (t <= 0) ?			pa.getModule() : 
			(0 < t && t < 1) ? (vector - (d.scalarMult(t) + _orig)).getModule() : 
								(vector - _dest).getModule();
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

bool SegmentLine::intersects(const Line& r, Vect2d& res) 
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = r.getA();
	Vect2d lineDest = r.getB();

	if (intersects(lineOrig, lineDest, t, s) && 
		0 <= s && s <= 1)
	{
		res = getPoint(s);
		return true;
	}

	return false;
}

bool SegmentLine::intersects(const RayLine& r, Vect2d& res) 
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = r.getA();
	Vect2d lineDest = r.getB();

	if (intersects(lineOrig, lineDest, t, s) && 
		0 <= t && 
		0 <= s && s <= 1)
	{
		res = getPoint(s);
		return true;
	}

	return false;
}

bool SegmentLine::intersects(const SegmentLine& r, Vect2d& res)
{
	float s = 0;
	float t = 0;

	Vect2d lineOrig = r.getA();
	Vect2d lineDest = r.getB();

	if (intersects(lineOrig, lineDest, t, s) && 
		(0 <= s && s <= 1) && 
		(0 <= t && t <= 1))
	{
		res = getPoint(s);
		return true;
	}

	return false;
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
	Vect2d d(_dest.getX() - _orig.getX(), _dest.getY() - _orig.getY());
	Vect2d pa = point - _orig;

	return (d.dot(pa) /
		d.getModule());
}