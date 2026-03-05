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


/*
 * File:   Ray3d.h
 * Author: lidia
 *
 * Created on 26 de enero de 2021, 17:22
 */

#pragma once

#include "Edge3d.h"

 /**
 *	@brief This class represents a ray that goes from an origin to the infinite.
 *	@author Lidia Mª Ortega Alvarado.
 */
class RayLine3d : public Edge3d
{
protected:
	/**
	*	@brief Checks if the parametric value t is valid. It is only invalid if t is smaller than 0.
	*/
	virtual bool isTvalid(double t) { return (t >= 0); }

public:
	RayLine3d() = default;

	/**
	*	@brief Constructor.
	*/
	RayLine3d(Vect3d& orig, Vect3d& dest);

	/**
	*	@brief Copy constructor.
	*/
	RayLine3d(const RayLine3d& ray);

	/**
	*	@brief Destructor.
	*/
	virtual ~RayLine3d();

	/**
	*	@brief Obtains de minimum ray-point distance
	*/

	double distance(Vect3d& v);

	/**
	*	@brief Assignment operator.
	*/
	virtual RayLine3d& operator=(const RayLine3d& ray);

	/**
	*	@brief Shows some information about the ray at the debug window.
	*/
	friend std::ostream& operator<<(std::ostream& os, const RayLine3d& ray);
};