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

#include "Vect3d.h"

/*
 * File:   AABB.h
 * Author: lidia
 *
 * Created on 25 de enero de 2021, 19:10
 */

class AABB
{
protected:
	Vect3d _min, _max;

public:
	/**
	*	@brief Default constructor.
	*/
	AABB();

	/**
	*	@brief Constructor.
	*/
	AABB(const Vect3d& min, const Vect3d& max);

	/**
	*	@brief Copy constructor,
	*/
	AABB(const AABB& aabb);

	AABB(const vec3& min, const vec3& max);

	/**
	*	@brief Destructor.
	*/
	virtual ~AABB();

	/**
	*	@brief Returns the central point of the cube.
	*/
	Vect3d getCenter();

	/**
	*	@brief Returns the vector that goes from the center to the maximum point.
	*/
	Vect3d getExtent();

	/**
	*	@brief Returns the lowest corner of the cube.
	*/
	Vect3d getMin() const { return _min; } 

	/**
	*	@brief Returns the maximum points of the cube.
	*/
	Vect3d getMax() const { return _max; } 

	/**
	*	@brief Assignment operator.
	*/
	AABB& operator=(const AABB& orig);

	/**
	*	@brief Shows some information of the vector as debugging info.
	*/
	friend std::ostream& operator<<(std::ostream& os, const AABB& aabb);

	/**
	*	@brief Modifies the minimum point.
	*/
	void setMin(const Vect3d& min) { _min = min; }

	/**
	*	@brief Modifies the maximum point.
	*/
	void setMax(const Vect3d& max) { _max = max; }

	Vect3d getSize() { return _max.sub(_min); }

	AABB dot(const mat4& matrix) const;

	void update(const AABB& aabb);

	void update(const vec3& point);
};