#include "stdafx.h"
#include "BasicGeometry.h"
#include "Triangle3d.h"
#include "Edge3d.h"


/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)											\
p0 = a * v0.getY() - b * v0.getZ();			       							\
p2 = a * v2.getY() - b * v2.getZ();			       							\
if (p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; }			\
rad = fa * boxHalfSize.getY() + fb * boxHalfSize.getZ();					\
if (min > rad || max < -rad) return false;
	
#define AXISTEST_X2(a, b, fa, fb)											\
p0 = a * v0.getY() - b * v0.getZ();											\
p1 = a * v1.getY() - b * v1.getZ();			       							\
if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
rad = fa * boxHalfSize.getY() + fb * boxHalfSize.getZ();					\
if (min > rad || max < -rad) return false;


/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)											\
p0 = -a * v0.getX() + b * v0.getZ();		      							\
p2 = -a * v2.getX() + b * v2.getZ();						  	       		\
if (p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; }			\
rad = fa * boxHalfSize.getX() + fb * boxHalfSize.getZ();					\
if (min > rad || max < -rad) return false;


#define AXISTEST_Y1(a, b, fa, fb)											\
p0 = -a * v0.getX() + b * v0.getZ();		      							\
p1 = -a * v1.getX() + b * v1.getZ();							       		\
if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
rad = fa * boxHalfSize.getX() + fb * boxHalfSize.getZ();					\
if (min > rad || max < -rad) return false;


/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)											\
p1 = a * v1.getX() - b * v1.getY();											\
p2 = a * v2.getX() - b * v2.getY();			       							\
if (p2 < p1) { min = p2; max = p1; } else { min = p1; max = p2; }			\
rad = fa * boxHalfSize.getX() + fb * boxHalfSize.getY();					\
if (min > rad || max < -rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)											\
p0 = a * v0.getX() - b * v0.getY();												\
p1 = a * v1.getX() - b * v1.getY();												\
if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
rad = fa * boxHalfSize.getX() + fb * boxHalfSize.getY();					\
if (min > rad || max < -rad) return false;

/*======================== end-tests ========================*/

Triangle3d::Triangle3d()
{
}

Triangle3d::Triangle3d(double ax, double ay, double az, double bx, double by, double bz, double cx, double cy, double cz)
	: _a(ax, ay, az), _b(bx, by, bz), _c(cx, cy, cz)
{
}

Triangle3d::Triangle3d(const Triangle3d& triangle)
	: _a(triangle._a), _b(triangle._b), _c(triangle._c)
{
}

Triangle3d::Triangle3d(Vect3d& va, Vect3d& vb, Vect3d& vc)
	: _a(va), _b(vb), _c(vc)
{
}

Triangle3d::~Triangle3d()
{
}

double Triangle3d::area()
{
	Vect3d sideC = _a.sub(_b);
	Vect3d sideB = _a.sub(_c);

	return sideC.xProduct(sideB).module();
}

Triangle3d::PointPosition Triangle3d::classify(Vect3d& point)
{
	Vect3d n = normal();
	Vect3d v = point.sub(_a);

	if (BasicGeometry::equal(v.module(), 0.0))
		return PointPosition::COPLANAR;

	v = v.normalize();
	double d = v.dot(n);

	if (d > BasicGeometry::EPSILON)
		return PointPosition::NEGATIVE;
	else if (d < -BasicGeometry::EPSILON)
		return PointPosition::POSITIVE;

	return PointPosition::COPLANAR;
}

bool Triangle3d::rayTri(RayLine3d& r, Vect3d& interVect) const
{
	Vect3d rOrig = r.getOrigin();
	Vect3d rDir = r.getDestination().sub(rOrig);
	Vect3d edge1 = _b.sub(_a);
	Vect3d edge2 = _c.sub(_a);

	// Backface culling, assuming CCW-wound triangles.
	const Vect3d normal = edge1.xProduct(edge2); // No need to normalize
	if (normal.dot(rDir) > 0)
		return false;

	Vect3d ray_cross_e2 = rDir.xProduct(edge2);
	float det = edge1.dot(ray_cross_e2);

	if (abs(det) < BasicGeometry::EPSILON) 
		return false; // Ray is parallel to triangle

	float inv_det = 1.0 / det;
	Vect3d s = rOrig.sub(_a);
	float u = inv_det * s.dot(ray_cross_e2);

	if (u < -BasicGeometry::EPSILON || u - 1 > BasicGeometry::EPSILON) 
		return false; // Ray passes outside edge2's bounds

	Vect3d s_cross_e1 = s.xProduct(edge1);
	float v = inv_det * rDir.dot(s_cross_e1);

	if (v < -BasicGeometry::EPSILON || u + v - 1 > BasicGeometry::EPSILON) 
		return false; // Ray passes outside edge1's bounds

	// The ray line intersects with the triangle.
	// We compute t to find where on the ray the intersection is.
	float t = inv_det * edge2.dot(s_cross_e1);

	if (t > BasicGeometry::EPSILON) // Ray intersection
	{
		interVect = (rDir.scalarMul(t).add(rOrig));
		return true;
	}

	return false;
}

bool Triangle3d::triAABB(AABB& aabb) const
{
	/*    use separating axis theorem to test overlap between triangle and box */
	/*    need to test for overlap in these directions: */
	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
	/*       we do not even need to test these) */
	/*    2) normal of the triangle */
	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */
	/*       this gives 3x3=9 more tests */
	Vect3d v0, v1, v2;
	Vect3d boxCenter = aabb.getCenter();
	Vect3d boxHalfSize = aabb.getExtent();

	//   float axis[3];
	float min, max, p0, p1, p2, rad, fex, fey, fez;		// -NJMP- "d" local variable removed
	Vect3d normal, e0, e1, e2;

	/* This is the fastest branch on Sun */

	/* move everything so that the boxcenter is in (0,0,0) */
	v0 = _a.sub(boxCenter);
	v1 = _b.sub(boxCenter);
	v2 = _c.sub(boxCenter);

	/* compute triangle edges */
	e0 = v1.sub(v0);		/* tri edge 0 */
	e1 = v2.sub(v1);		/* tri edge 1 */
	e2 = v0.sub(v2); 		/* tri edge 2 */

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */

	fex = fabsf(e0.getX());
	fey = fabsf(e0.getY());
	fez = fabsf(e0.getZ());
	AXISTEST_X01(e0.getZ(), e0.getY(), fez, fey);
	AXISTEST_Y02(e0.getZ(), e0.getX(), fez, fex);
	AXISTEST_Z12(e0.getY(), e0.getX(), fey, fex);

	fex = fabsf(e1.getX());
	fey = fabsf(e1.getY());
	fez = fabsf(e1.getZ());
	AXISTEST_X01(e1.getZ(), e1.getY(), fez, fey);
	AXISTEST_Y02(e1.getZ(), e1.getX(), fez, fex);
	AXISTEST_Z0(e1.getY(), e1.getX(), fey, fex);

	fex = fabsf(e2.getX());
	fey = fabsf(e2.getY());
	fez = fabsf(e2.getZ());
	AXISTEST_X2(e2.getZ(), e2.getY(), fez, fey);
	AXISTEST_Y1(e2.getZ(), e2.getX(), fez, fex);
	AXISTEST_Z12(e2.getY(), e2.getX(), fey, fex);

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */

	min = (v0.getX() < v1.getX()) ? 
			((v0.getX() < v2.getX()) ? v0.getX() : v2.getX()) : 
			((v1.getX() < v2.getX()) ? v1.getX() : v2.getX());
	max = (v0.getX() > v1.getX()) ?
			((v0.getX() > v2.getX()) ? v0.getX() : v2.getX()) :
			((v1.getX() > v2.getX()) ? v1.getX() : v2.getX());
	if (min > boxHalfSize.getX() || max < -boxHalfSize.getX()) return false;
	
	/* test in Y-direction */
	min = (v0.getY() < v1.getY()) ?
		((v0.getY() < v2.getY()) ? v0.getY() : v2.getY()) :
		((v1.getY() < v2.getY()) ? v1.getY() : v2.getY());
	max = (v0.getY() > v1.getY()) ?
		((v0.getY() > v2.getY()) ? v0.getY() : v2.getY()) :
		((v1.getY() > v2.getY()) ? v1.getY() : v2.getY());
	if (min > boxHalfSize.getY() || max < -boxHalfSize.getY()) return false;

	/* test in Z-direction */
	min = (v0.getZ() < v1.getZ()) ?
		((v0.getZ() < v2.getZ()) ? v0.getZ() : v2.getZ()) :
		((v1.getZ() < v2.getZ()) ? v1.getZ() : v2.getZ());
	max = (v0.getZ() > v1.getZ()) ?
		((v0.getZ() > v2.getZ()) ? v0.getZ() : v2.getZ()) :
		((v1.getZ() > v2.getZ()) ? v1.getZ() : v2.getZ());
	if (min > boxHalfSize.getZ() || max < -boxHalfSize.getZ()) return false;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	normal = e0.xProduct(e1);

	// -NJMP- (line removed here)
	if (!planeBoxOverlap(normal, v0, boxHalfSize)) return false;	// -NJMP-

	return true;   /* box and triangle overlaps */
}

Vect3d Triangle3d::normal()
{
	Vect3d v1(_b.sub(_a));
	Vect3d v2(_c.sub(_a));
	Vect3d n(v1.xProduct(v2));
	double module = n.module();

	return (n.scalarMul(1.0f / module));
}

Triangle3d& Triangle3d::operator=(const Triangle3d& triangle)
{
	_a = triangle._a;
	_b = triangle._b;
	_c = triangle._c;

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Triangle3d& triangle)
{
	std::cout << "Triangle 3D -> " << triangle._a << ", " << triangle._b << ", " << triangle._c;
	return os;
}

void Triangle3d::set(Vect3d& va, Vect3d& vb, Vect3d& vc)
{
	_a = va;
	_b = vb;
	_c = vc;
}

bool Triangle3d::planeBoxOverlap(Vect3d normal, Vect3d vert, Vect3d maxbox)	const // -NJMP-
{
	int q;
	float v;
	Vect3d vmin, vmax;
	for (q = 0; q <= 2; q++)
	{
		v = vert.get(q);					// -NJMP-
		if (normal.get(q) > 0.0f)
		{
			vmin.set(q, -maxbox.get(q) - v);	// -NJMP-
			vmax.set(q, maxbox.get(q) - v);		// -NJMP-
		}
		else
		{
			vmin.set(q, maxbox.get(q) - v);		// -NJMP-
			vmax.set(q, -maxbox.get(q) - v);	// -NJMP-
		}
	}

	if (normal.dot(vmin) > 0.0f) return false;		// -NJMP-
	if (normal.dot(vmax) >= 0.0f) return true;	// -NJMP-

	return false;
}