/*   Copyright (C) 2023 Lidia Ortega Alvarado, Alfonso Lopez Ruiz
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see https://github.com/AlfonsoLRz/AG2223.
*/

#pragma once

#include "Edge3d.h"

/**
*	@brief This class represent a segment formed by two points.
*	@author Lidia Mª Ortega Alvarado.
*/
class SegmentLine3d : public Edge3d
{
protected:
	/**
	*	@brief Checks if the parametric value t is between 0 and 1 as a segment is not infinite.
	*/
	virtual bool isTvalid(double t) { return (t >= 0.0f && t <= 1.0f); }

public:
	/**
	*	@brief Creates a segment with default point values.
	*/
	SegmentLine3d() {};

	/**
	*	@brief Constructor.
	*/
	SegmentLine3d(Vect3d& orig, Vect3d& dest);

	/**
	*	@brief Copy constructor.
	*/
	SegmentLine3d(const SegmentLine3d& segment);

	/**
	*	@brief Destructor.
	*/
	virtual ~SegmentLine3d();

	/**
*	@brief Obtains de minimum segment-point distance
*/
	double distance(Vect3d& v);

	/**
	*	@brief Assignment operator.
	*/
	virtual SegmentLine3d& operator=(const SegmentLine3d& segment);

	/**
	*	@brief Shows the segment data at the debug window.
	*/
	friend std::ostream& operator<<(std::ostream& os, const SegmentLine3d& segment);
};