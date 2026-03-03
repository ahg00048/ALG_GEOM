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

Line3d Line3d::normalLine(Vect3d& point)
{
	Vect3d tDir = _dest.sub(_orig);
	Vect3d pt = point.sub(_orig);
	double lambda = tDir.dot(pt) / tDir.dot(tDir);
	Vect3d point2 = tDir.scalarMul(lambda);

	return Line3d(point, point2);
}

double Line3d::distance(Vect3d& p) {
	Vect3d tDir = _dest.sub(_orig);
	Vect3d pt = p.sub(_orig);
	double lambda = tDir.dot(pt) / tDir.dot(tDir);
	Vect3d point2 = tDir.scalarMul(lambda);
	
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

