#include "stdafx.h"
#include "Edge3d.h"
#include "RayLine3d.h"

RayLine3d::RayLine3d(Vect3d& orig, Vect3d& dest)
	: Edge3d(orig, dest)
{
}

RayLine3d::RayLine3d(const RayLine3d& ray)
	: Edge3d(ray)
{
}

RayLine3d::~RayLine3d()
{
}

RayLine3d& RayLine3d::operator=(const RayLine3d& ray)
{
	if (this != &ray)
	{
		Edge3d::operator=(ray);
	}

	return *this;
}

std::ostream& operator<<(std::ostream& os, const RayLine3d& ray)
{
	os << "Ray --> Origin: " << ray._orig << "; destination: " << ray._dest;
	return os;
}

double RayLine3d::distance(Vect3d& v) {
	double t = 0.0;
	double distance = 0.0;


	Vect3d tDir = _dest.sub(_orig);
	Vect3d pt = v.sub(_orig);
	t = tDir.dot(pt) / tDir.dot(tDir);

	if (t < 0.0)
	{
		distance = v.sub(_orig).module();
	}
	else
	{
		Vect3d v2 = tDir.scalarMul(t).add(_orig);

		distance = v.sub(v2).module();
	}

	return distance;
}