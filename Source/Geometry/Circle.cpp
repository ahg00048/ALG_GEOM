#include <cstdint>

#include "stdafx.h"
#include "Circle.h"
#include "Point.h"
#include "Polygon.h"

// Public methods

Circle::~Circle()
{
}

bool Circle::isInside(const Point &p)
{ 
    return _center.distance(p) <= _radius;
}

Polygon Circle::getPointsCircle(uint32_t numSubdivisions)
{        
    Polygon polygon;
    float angle = .0f, alpha = 2.0f * glm::pi<float>() / numSubdivisions;

    while (angle < (2.0f * glm::pi<float>() + glm::epsilon<float>()))
    {
        Point point = Point((_center.getX() + std::cos(angle) * _radius), (_center.getY() - std::sin(angle) * _radius));
        polygon.add(point);
        angle += alpha;
    }

    return polygon;
}

Circle& Circle::operator=(const Circle& circle)
{
    if (this != &circle)
    {
        this->_center = circle._center;
        this->_radius = circle._radius;
    }

    return *this;
}
    
const Circle::RelationCircles Circle::relationCircle(const Circle& c) const 
{
    double cDist = Point(_center.getX() - c._center.getX(), _center.getY() - c._center.getY()).getModule();
    double rSum = _radius + c._radius;
    double rSub = fabs(_radius - c._radius);

    if (BasicGeometry::equal(cDist, 0))
        return Circle::RelationCircles::CONCENTRIC;
    else if (BasicGeometry::equal(cDist, rSum))
        return Circle::RelationCircles::EXTERNAL_TANG; 
    else if (cDist > rSum)
        return Circle::RelationCircles::EXTERNAL;
    else if (cDist < rSub)
        return Circle::RelationCircles::INTERNAL;
    else if (rSub < cDist < rSum)
        return Circle::RelationCircles::SECANT;

    return Circle::RelationCircles::INTERIOR_TANG;
}

const Circle::RelationCircleLine Circle::relationLine(Line& l) const
{
    Vect2d vCenter = _center;
    double dist = l.distancePointLine(vCenter);

    if (BasicGeometry::equal(dist, _radius))
        return Circle::RelationCircleLine::TANGENTS;
    else if (dist < _radius)
        return Circle::RelationCircleLine::INTERSECT;
    
    return Circle::RelationCircleLine::NO_INTERSECT;
}

const Circle::RelationCircleLine Circle::intersect(Line& l, Vect2d& v1, Vect2d& v2) const
{
    Vect2d lineP = l.getA();
    Vect2d lineQ = l.getB();
    Vect2d d = lineQ - lineP;
    Vect2d diff = lineP - _center;

    double delta = pow(d.dot(diff), 2) - pow(d.getModule(), 2) * (pow(diff.getModule(), 2) - pow(_radius, 2));
    double t1 = -(d.dot(diff) + sqrt(delta)) / pow(d.getModule(), 2);
    double t2 = -(d.dot(diff) + sqrt(delta)) / pow(d.getModule(), 2);

    if (BasicGeometry::equal(delta, 0))
    {
        v1 = l.getPoint(t1);
        return Circle::RelationCircleLine::TANGENTS;
    }
    else if (delta > 0)
    {
        v1 = l.getPoint(t1);
        v2 = l.getPoint(t2);
        return Circle::RelationCircleLine::INTERSECT;
    }

    return Circle::RelationCircleLine::NO_INTERSECT;
}

const Circle::RelationCircleLine Circle::intersect(SegmentLine& sg, Vect2d& v1, Vect2d& v2) const
{
    Vect2d lineP = sg.getA();
    Vect2d lineQ = sg.getB();
    Vect2d d = lineQ - lineP;
    Vect2d diff = lineP - _center;

    double delta = pow(d.dot(diff), 2) - pow(d.getModule(), 2) * (pow(diff.getModule(), 2) - pow(_radius, 2));
    double t1 = -(d.dot(diff) + sqrt(delta)) / pow(d.getModule(), 2);
    double t2 = -(d.dot(diff) + sqrt(delta)) / pow(d.getModule(), 2);

    bool insideSgT1 = (t1 <= 1 && t1 >= 0);
    bool insideSgT2 = (t2 <= 1 && t2 >= 0);

    if (BasicGeometry::equal(delta, 0) && insideSgT1)
    {
        v1 = sg.getPoint(t1);   
        return Circle::RelationCircleLine::TANGENTS;
    }
    else if (delta > 0 && (insideSgT1 || insideSgT2))
    {
        if (insideSgT1)
        {
            v1 = sg.getPoint(t1);
       
            if (insideSgT2) v2 = sg.getPoint(t2);
        }
        else
        {
            v1 = sg.getPoint(t2);
        }

        return Circle::RelationCircleLine::INTERSECT;
    }

    return Circle::RelationCircleLine::NO_INTERSECT;
}

const Circle::RelationCircleLine Circle::intersect(RayLine& r, Vect2d& v1, Vect2d& v2) const
{
    Vect2d lineP = r.getA();
    Vect2d lineQ = r.getB();
    Vect2d d = lineQ - lineP;
    Vect2d diff = lineP - _center;

    double delta = pow(d.dot(diff), 2) - pow(d.getModule(), 2) * (pow(diff.getModule(), 2) - pow(_radius, 2));
    double t1 = -(d.dot(diff) + sqrt(delta)) / pow(d.getModule(), 2);
    double t2 = -(d.dot(diff) + sqrt(delta)) / pow(d.getModule(), 2);

    v1 = r.getPoint(t1);
    v2 = r.getPoint(t2);

    bool insideRayT1 = (t1 >= 0);
    bool insideRayT2 = (t2 >= 0);

    if (BasicGeometry::equal(delta, 0) && insideRayT1)
    {
        v1 = r.getPoint(t1);
        return Circle::RelationCircleLine::TANGENTS;
    }
    else if (delta > 0 && (insideRayT1 || insideRayT2))
    {
        if (insideRayT1)
        {
            v1 = r.getPoint(t1);

            if (insideRayT2) v2 = r.getPoint(t2);
        }
        else
        {
            v1 = r.getPoint(t2);
        }
        return Circle::RelationCircleLine::INTERSECT;
    }

    return Circle::RelationCircleLine::NO_INTERSECT;
}