#include "stdafx.h"
#include "BasicGeometry.h"
#include "Triangle3d.h"
#include "Edge3d.h"

/*======================= AABB-TRI ========================*/

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
p0 = a * v0.getX() - b * v0.getY();											\
p1 = a * v1.getX() - b * v1.getY();											\
if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
rad = fa * boxHalfSize.getX() + fb * boxHalfSize.getY();					\
if (min > rad || max < -rad) return false;

/*======================== end-tests ========================*/

/*========================= tri-tri =========================*/

#define NEWCOMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,A,B,C,X0,X1)				\
{																						\
        if(D0D1>0.0f)																	\
        {																				\
            /* here we know that D0D2<=0.0 */											\
            /* that is D0, D1 are on the same side, D2 on the other or on the plane */	\
            A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1;					\
        }																				\
        else if(D0D2>0.0f)																\
        {																				\
                /* here we know that d0d1<=0.0 */										\
	        A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2;					\
        }																				\
        else if(D1*D2>0.0f || D0!=0.0f)													\
        {																				\
            /* here we know that d0d1<=0.0 or that D0!=0.0 */							\
            A=VV0; B=(VV1-VV0)*D0; C=(VV2-VV0)*D0; X0=D0-D1; X1=D0-D2;					\
        }																				\
        else if(D1!=0.0f)																\
        {																				\
            A=VV1; B=(VV0-VV1)*D1; C=(VV2-VV1)*D1; X0=D1-D0; X1=D1-D2;					\
        }																				\
        else if(D2!=0.0f)																\
        {																				\
            A=VV2; B=(VV0-VV2)*D2; C=(VV1-VV2)*D2; X0=D2-D0; X1=D2-D1;					\
        }																				\
        else																			\
        {																				\
            /* triangles are coplanar */												\
            return coplanar_tri_tri(N1, aAux, bAux, cAux, otherA, otherB, otherC);		\
        }																				\
}

/*========================= EDGE-EDGE =========================*/

#define EDGE_EDGE_TEST(V0,U0,U1)                      \
  Bx=U0.get(i0)-U1.get(i0);                           \
  By=U0.get(i1)-U1.get(i1);                           \
  Cx=V0.get(i0)-U0.get(i0);                           \
  Cy=V0.get(i1)-U0.get(i1);                           \
  f=Ay*Bx-Ax*By;                                      \
  d=By*Cx-Bx*Cy;                                      \
  if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
  {                                                   \
    e=Ax*Cy-Ay*Cx;                                    \
    if(f>0)                                           \
    {                                                 \
      if(e>=0 && e<=f) return 1;                      \
    }                                                 \
    else                                              \
    {                                                 \
      if(e<=0 && e>=f) return 1;                      \
    }                                                 \
  }

/*========================= EDGE-EDGE =========================*/

#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2)	\
{												\
  float Ax,Ay,Bx,By,Cx,Cy,e,d,f;				\
  Ax=V1.get(i0)-V0.get(i0);						\
  Ay=V1.get(i1)-V0.get(i1);						\
  /* test edge U0,U1 against V0,V1 */			\
  EDGE_EDGE_TEST(V0,U0,U1);						\
  /* test edge U1,U2 against V0,V1 */			\
  EDGE_EDGE_TEST(V0,U1,U2);						\
  /* test edge U2,U1 against V0,V1 */			\
  EDGE_EDGE_TEST(V0,U2,U0);						\
}

/*========================= POINT =========================*/

#define POINT_IN_TRI(V0,U0,U1,U2)					\
{													\
  float a,b,c,d0,d1,d2;								\
  /* is T1 completly inside T2? */					\
  /* check if V0 is inside tri(U0,U1,U2) */			\
  a=U1.get(i1)-U0.get(i1);                          \
  b=-(U1.get(i0)-U0.get(i0));                       \
  c=-a*U0.get(i0)-b*U0.get(i1);                     \
  d0=a*V0.get(i0)+b*V0.get(i1)+c;                   \
													\
  a=U2.get(i1)-U1.get(i1);                          \
  b=-(U2.get(i0)-U1.get(i0));                       \
  c=-a*U1.get(i0)-b*U1.get(i1);                     \
  d1=a*V0.get(i0)+b*V0.get(i1)+c;                   \
									                \
  a=U0.get(i1)-U2.get(i1);                          \
  b=-(U0.get(i0)-U2.get(i0));                       \
  c=-a*U2.get(i0)-b*U2.get(i1);                     \
  d2=a*V0.get(i0)+b*V0.get(i1)+c;                   \
  if(d0*d1>0.0)										\
  {													\
    if(d0*d2>0.0) return 1;							\
  }													\
}

/*========================================================================*/

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
	//if (normal.dot(rDir) > 0)
	//	return false;

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

AABB Triangle3d::getAABB() const
{
	double xMin = ((_a.getX() < _b.getX()) ?
		((_a.getX() < _c.getX()) ? _a.getX() : _c.getX()) :
		((_b.getX() < _c.getX()) ? _b.getX() : _c.getX()));
	double yMin = ((_a.getY() < _b.getY()) ?
		((_a.getY() < _c.getY()) ? _a.getY() : _c.getY()) :
		((_b.getY() < _c.getY()) ? _b.getY() : _c.getY()));
	double zMin = ((_a.getZ() < _b.getZ()) ?
		((_a.getZ() < _c.getZ()) ? _a.getZ() : _c.getZ()) :
		((_b.getZ() < _c.getZ()) ? _b.getZ() : _c.getZ()));

	double xMax = ((_a.getX() > _b.getX()) ?
		((_a.getX() > _c.getX()) ? _a.getX() : _c.getX()) :
		((_b.getX() > _c.getX()) ? _b.getX() : _c.getX()));
	double yMax = ((_a.getY() > _b.getY()) ?
		((_a.getY() > _c.getY()) ? _a.getY() : _c.getY()) :
		((_b.getY() > _c.getY()) ? _b.getY() : _c.getY()));
	double zMax = ((_a.getZ() > _b.getZ()) ?
		((_a.getZ() > _c.getZ()) ? _a.getZ() : _c.getZ()) :
		((_b.getZ() > _c.getZ()) ? _b.getZ() : _c.getZ()));

	return AABB(Vect3d(xMin, yMin, zMin), Vect3d(xMax, yMax, zMax));
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

bool Triangle3d::operator==(const Triangle3d& triangle) const
{
	int counter = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (get(i) == triangle.get(j))
			{
				counter++;
			}
		}
	}

	return counter == 3;
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

bool Triangle3d::triTri(Triangle3d& tri) const
{
	Vect3d E1, E2;
	Vect3d N1, N2;
	float d1, d2;
	float du0, du1, du2, dv0, dv1, dv2;
	Vect3d D;
	float isect1[2], isect2[2];
	float du0du1, du0du2, dv0dv1, dv0dv2;
	short index;
	float vp0, vp1, vp2;
	float up0, up1, up2;
	float bb, cc, max;

	Vect3d aAux = _a;
	Vect3d bAux = _b;
	Vect3d cAux = _c;

	/* compute plane equation of triangle(V0,V1,V2) */
	E1 = _b.sub(_a);
	E2 = _c.sub(_a);
	N1 = E1.xProduct(E2);

	d1 = -N1.dot(aAux);
	/* plane equation 1: N1.X+d1=0 */

	/* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
	Vect3d otherA = tri.getA();
	Vect3d otherB = tri.getB();
	Vect3d otherC = tri.getC();
	
	du0 = N1.dot(otherA) + d1;
	du1 = N1.dot(otherB) + d1;
	du2 = N1.dot(otherC) + d1;

	/* coplanarity robustness check */

	if (fabsf(du0) < BasicGeometry::EPSILON) du0 = 0.0;
	if (fabsf(du1) < BasicGeometry::EPSILON) du1 = 0.0;
	if (fabsf(du2) < BasicGeometry::EPSILON) du2 = 0.0;

	du0du1 = du0 * du1;
	du0du2 = du0 * du2;

	if (du0du1 > 0.0f && du0du2 > 0.0f) /* same sign on all of them + not equal 0 ? */
		return false;                    /* no intersection occurs */

	/* compute plane of triangle (U0,U1,U2) */
	E1 = otherB.sub(otherA);
	E2 = otherC.sub(otherA);
	N2 = E1.xProduct(E2);
	d2 = -N2.dot(otherA);
	/* plane equation 2: N2.X+d2=0 */

	/* put V0,V1,V2 into plane equation 2 */
	dv0 = N2.dot(aAux) + d2;
	dv1 = N2.dot(bAux) + d2;
	dv2 = N2.dot(cAux) + d2;

	if (fabsf(dv0) < BasicGeometry::EPSILON) dv0 = 0.0;
	if (fabsf(dv1) < BasicGeometry::EPSILON) dv1 = 0.0;
	if (fabsf(dv2) < BasicGeometry::EPSILON) dv2 = 0.0;

	dv0dv1 = dv0 * dv1;
	dv0dv2 = dv0 * dv2;

	if (dv0dv1 > 0.0f && dv0dv2 > 0.0f) /* same sign on all of them + not equal 0 ? */
		return false;                    /* no intersection occurs */

	/* compute direction of intersection line */
	D = N1.xProduct(N2);

	/* compute and index to the largest component of D */
	max = fabsf(D.getX());
	index = 0;
	bb = fabsf(D.getY());
	cc = fabsf(D.getZ());
	if (bb > max) max = bb, index = 1;
	if (cc > max) max = cc, index = 2;

	/* this is the simplified projection onto L*/
	vp0 = aAux.get(index);
	vp1 = bAux.get(index);
	vp2 = cAux.get(index);

	up0 = otherA.get(index);
	up1 = otherB.get(index);
	up2 = otherC.get(index);

	/* compute interval for triangle 1 */
	float a, b, c, x0, x1;
	NEWCOMPUTE_INTERVALS(vp0, vp1, vp2, dv0, dv1, dv2, dv0dv1, dv0dv2, a, b, c, x0, x1);

	/* compute interval for triangle 2 */
	float d, e, f, y0, y1;
	NEWCOMPUTE_INTERVALS(up0, up1, up2, du0, du1, du2, du0du1, du0du2, d, e, f, y0, y1);

	float xx, yy, xxyy, tmp;
	xx = x0 * x1;
	yy = y0 * y1;
	xxyy = xx * yy;

	tmp = a * xxyy;
	isect1[0] = tmp + b * x1 * yy;
	isect1[1] = tmp + c * x0 * yy;

	tmp = d * xxyy;
	isect2[0] = tmp + e * xx * y1;
	isect2[1] = tmp + f * xx * y0;

	if (isect1[0] > isect1[1])
	{          
		float c; 
		c = isect1[0];
		isect1[0] = isect1[1];
		isect1[1] = c;
	}

	if (isect2[0] > isect2[1])
	{          
		float c; 
		c = isect2[0];
		isect2[0] = isect2[1];
		isect2[1] = c;
	}

	if (isect1[1] < isect2[0] || isect2[1] < isect1[0]) return false;
	
	return true;
}

bool Triangle3d::coplanar_tri_tri(Vect3d N, Vect3d V0, Vect3d V1, Vect3d V2,
	Vect3d U0, Vect3d U1, Vect3d U2) const
{
	Vect3d A;
	short i0, i1;
	/* first project onto an axis-aligned plane, that maximizes the area */
	/* of the triangles, compute indices: i0,i1. */
	A.setX(fabs(N.getX()));
	A.setY(fabs(N.getY()));
	A.setZ(fabs(N.getZ()));

	if (A.getX() > A.getY())
	{
		if (A.getX() > A.getZ())
		{
			i0 = 1;      /* A[0] is greatest */
			i1 = 2;
		}
		else
		{
			i0 = 0;      /* A[2] is greatest */
			i1 = 1;
		}
	}
	else   /* A[0]<=A[1] */
	{
		if (A.getZ() > A.getY())
		{
			i0 = 0;      /* A[2] is greatest */
			i1 = 1;
		}
		else
		{
			i0 = 0;      /* A[1] is greatest */
			i1 = 2;
		}
	}

	/* test all edges of triangle 1 against the edges of triangle 2 */
	EDGE_AGAINST_TRI_EDGES(V0, V1, U0, U1, U2);
	EDGE_AGAINST_TRI_EDGES(V1, V2, U0, U1, U2);
	EDGE_AGAINST_TRI_EDGES(V2, V0, U0, U1, U2);

	/* finally, test if tri1 is totally contained in tri2 or vice versa */
	POINT_IN_TRI(V0, U0, U1, U2);
	POINT_IN_TRI(U0, V0, V1, V2);

	return false;
}

void Triangle3d::translate(Vect3d translation)
{
	_a = _a.add(translation);
	_b = _b.add(translation);
	_c = _c.add(translation);
}

void Triangle3d::changeNormalDir()
{
	Vect3d aux = _b;
	_b = _a;
	_a = aux;
}

Vect3d Triangle3d::get(int index) const
{
	switch (index)
	{
	case 0:
		return _a;
		break;
	case 1:
		return _b;
		break;
	case 2:
		return _c;
		break;
	default:
		return Vect3d();
	}
}