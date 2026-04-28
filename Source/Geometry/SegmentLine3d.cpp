#include "stdafx.h"
#include "SegmentLine3d.h"
#include "BasicGeometry.h"

SegmentLine3d::SegmentLine3d(Vect3d& orig, Vect3d& dest)
	: Edge3d(orig, dest)
{
}

SegmentLine3d::SegmentLine3d(const SegmentLine3d& segment)
	: Edge3d(segment)
{
}

SegmentLine3d::~SegmentLine3d()
{
}

SegmentLine3d& SegmentLine3d::operator=(const SegmentLine3d& segment)
{
	if (this != &segment)
	{
		Edge3d::operator=(segment);
	}

	return *this;
}

std::ostream& operator<<(std::ostream& os, const SegmentLine3d& segment)
{
	std::cout << "Segment -> Origin: " << segment._orig << ", " << ", Destination: " << segment._dest;
	return os;
}

double SegmentLine3d::distance(Vect3d& v) {
	double t = 0.0;
	double distance = 0.0;

	Vect3d tDir = _dest.sub(_orig);
	Vect3d pt = v.sub(_orig);
	t = tDir.dot(pt) / tDir.dot(tDir);

	if (t < 0.0)
	{
		distance = v.sub(_orig).module();
	}
	else if(t > 1.0)
	{
		distance = v.sub(_dest).module();
	} 
	else
	{
		Vect3d v2 = tDir.scalarMul(t).add(_orig);

		distance = v.sub(v2).module();
	}

	return distance;
}


bool SegmentLine3d::operator==(const SegmentLine3d& segment) const
{
	return (_orig == segment._orig && _dest == segment._dest) || (_orig == segment._dest && _dest == segment._orig);
}



float SegmentLine3d::angle(SegmentLine3d& segment)
{
	Vect3d otherDest = segment._dest;

	Vect3d thisDir = _dest.sub(_orig);
	Vect3d otherDir = otherDest.sub(segment._orig);

	return acosf(thisDir.dot(otherDest) / (thisDir.module() * otherDir.module()));
}