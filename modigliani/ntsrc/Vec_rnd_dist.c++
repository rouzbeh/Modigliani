/**\file nt_vec_rnd_dist_obj.cpp - Vec_rnd_dist class implementation
 * by Ahmed Aldo Faisal &copy; created 26.6.1999
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.3
 * Copyright (C) 1998,199 Ahmed Aldo Faisal
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Vec_rnd_dist.h"
using namespace mbase;

/* ***      CONSTRUCTORS	***/

/** Create a Vec_rnd_dist */
Vec_rnd_dist::Vec_rnd_dist(Msize newDim) {
	dim = newDim;
	rndDistPtrVec.resize(dim);
}

/* ***      COPY AND ASSIGNMEM	***/
Vec_rnd_dist::Vec_rnd_dist(
		const Vec_rnd_dist __attribute__((unused)) & original) :
		Obj() {
}

const Vec_rnd_dist&
Vec_rnd_dist::operator=(const Vec_rnd_dist & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Vec_rnd_dist::~Vec_rnd_dist() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */

Vector Vec_rnd_dist::RndVec() const {
	Vector tmp(3); //2DO speed up using cache unit
	Msize ll;
	for (ll = 0; ll < dim; ll++) {
		tmp[ll] = (rndDistPtrVec[ll])->RndVal();
	}
	return (tmp);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenMObj version 0.4. */
