#include "stdafx.h"
#include "BasicGeometry.h"
#include "Line3d.h"

Line3d::Line3d()
{
}

Line3d::Line3d(Vect3d& orig, Vect3d& dest)
	: Edge3d(orig, dest)
{
}

Line3d::Line3d(const Line3d& line)
	: Edge3d(line)
{
}

Line3d::~Line3d()
{
}

double Line3d::distance(Line3d& line)
{
	Vect3d oOrig = line.getOrigin();

	Vect3d oDir = line.getDestination().sub(oOrig);
	Vect3d tDir = _dest.sub(_orig);
	Vect3d xProduct = tDir.xProduct(oDir);

	return (xProduct.module() != 0) ? 
		
		fabs((_orig.sub(oOrig)).dot(xProduct)) / 							
		xProduct.module() : 

		(_orig.sub(oOrig)).xProduct(tDir).module() / 
		tDir.module();
}

double Line3d::distance(SegmentLine3d& segm)
{
	Vect3d segOrig = segm.getOrigin();
	Vect3d lDir = (_dest.sub(_orig)).normalize();
	Vect3d segDir = (segm.getDestination().sub(segOrig)).normalize();
	
	Vect3d u = _orig.sub(segOrig);
	double a = lDir.dot(lDir);
	double b = lDir.dot(segDir);
	double c = segDir.dot(segDir);
	double d = lDir.dot(u);
	double e = segDir.dot(u);
	double det = a * c - b * b;
	
	double sDenom = det;
	double sNum = 0.0;
	double tNum = 0.0;
	double tDenom = 0.0;

	if (det < BasicGeometry::EPSILON)
	{
		sNum = 0.0;
		tNum = e;
		tDenom = c;
	} 
	else
	{
		sNum = b * e - c * d;
		tNum = a * e - b * d;
	}

	if(tNum < 0.0)
	{
		tNum = 0.0;
		sNum = -d;
		sDenom = a;
	}
	else if (tNum > tDenom)
	{
		tNum = tDenom;
		sNum = -d + b;
		sDenom = a;
	}

	double s = sNum / sDenom;
	double t = tNum / tDenom;

	Vect3d sLDir = lDir.scalarMul(s);
	Vect3d tSegDir = segDir.scalarMul(t);
	Vect3d A1 = _orig.add(sLDir);
	Vect3d A2 = segOrig.add(tSegDir);
	Vect3d v = A1.sub(A2);
	return v.module(); //dot?
}

Line3d Line3d::normalLine(Vect3d& point)
{
	Vect3d tDir = _dest.sub(_orig);
	Vect3d pt = point.sub(_orig);
	double lambda = tDir.dot(pt) / tDir.dot(tDir);
	
	Vect3d point2 = tDir.scalarMul(lambda).add(_orig);

	return Line3d(point, point2);
}

double Line3d::distance(Vect3d& p) {
	Vect3d tDir = _dest.sub(_orig);
	Vect3d pt = p.sub(_orig);
	double lambda = tDir.dot(pt) / tDir.dot(tDir);
	
	Vect3d point2 = tDir.scalarMul(lambda).add(_orig);
	
	return p.sub(point2).module();
}

Line3d& Line3d::operator=(const Line3d& line)
{
	if (this != &line)
	{
		Edge3d::operator=(line);
	}

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Line3d& line)
{
	std::cout << "Line -> Origin: " << line._orig << ", Destination: " << line._dest;
	return os;
}

