#pragma once

/* 
 *  File:   Circle.h
 *  Author: lidia
 *
 *  Created on 8 de febrero de 2021, 19:32
 */

#include "Point.h"
#include "Polygon.h"
#include "Line.h"

class Circle 
{
public:
    enum class RelationCircles
    {
        CONCENTRIC = 0,
        EXTERNAL, 
        INTERNAL,
        SECANT,
        INTERIOR_TANG,
        EXTERNAL_TANG
    };

    enum class RelationCircleLine
    {
        INTERSECT = 0,
        TANGENTS,
        NO_INTERSECT
    };

protected:
    Point       _center;             
    double      _radius;

public:
    /**
	*	@brief Default constructor. Coordinates are initialized with an invalid value.
	*/
    Circle (): _center(Point (0,0)), _radius(1.0) { }

    /**
    *	@brief Constructor.
	*/
    Circle(const Point &center, double radius): _center(center), _radius(radius) { }

    /**
	*	@brief Copy constructor.
	*/
    Circle (const Circle& orig): _center(orig._center), _radius(orig._radius) { }
    
	/**
	*	@brief Destructor.
	*/
    virtual ~Circle();

    /**
    *   @brief Checks if the point is inside the circle.
    *   @param p
    *   @return true if p is inside the circle 
    */  
    bool isInside (const Point &p);
    
    /**
    *   @return A polygon with circular shape.
    */
    Polygon getPointsCircle (uint32_t numSubdivisions = 256);

    /**
    *	@brief Assignment operator (override).
    */
    virtual Circle& operator=(const Circle& circle);

    const RelationCircles relationCircle(const Circle& c) const;

    const RelationCircleLine relationLine(Line& l) const;

    const RelationCircleLine intersect(Line& l, Vect2d& v1, Vect2d& v2) const;

    const RelationCircleLine intersect(SegmentLine& sg, Vect2d& v1, Vect2d& v2) const;

    const RelationCircleLine intersect(RayLine& r, Vect2d& v1, Vect2d& v2) const;
};

