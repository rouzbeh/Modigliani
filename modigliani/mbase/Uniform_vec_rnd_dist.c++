/**\file Uniform_vec_rnd_dist.c++ - Uniform_vec_rnd_dist class implementation
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

#include "Uniform_vec_rnd_dist.h"

using namespace mbase;

/* ***      CONSTRUCTORS	***/

/** dim-dimensional unit with the same distribution (parameters) in each dimension */
Uniform_vec_rnd_dist::Uniform_vec_rnd_dist(Real newLowerBound,
		Real newUpperBound, Size_t newDim) :
		Vec_rnd_dist(newDim), lowerBoundVec(newDim), upperBoundVec(newDim) {
	Uniform_rnd_dist * tmpPtr = new Uniform_rnd_dist(newLowerBound,
			newUpperBound);

	Size_t ll;
	for (ll = 0; ll < dim; ll++) {
		lowerBoundVec[ll] = newLowerBound;
		upperBoundVec[ll] = newUpperBound;
		rndDistPtrVec[ll] = tmpPtr;
	}

}

/** dim-dimensional unit with the individual distribution paramters in each dimension */
Uniform_vec_rnd_dist::Uniform_vec_rnd_dist(Vector newLowerBound,
		Vector newUpperBound, Size_t newDim) :
		Vec_rnd_dist(newDim), lowerBoundVec(newDim), upperBoundVec(newDim) {
	Size_t ll;
	M_ASSERT_DIM(newLowerBound._dimension(), dim);
	M_ASSERT_DIM(newUpperBound._dimension(), dim);

	lowerBoundVec = newLowerBound;
	upperBoundVec = newUpperBound;

	for (ll = 0; ll < dim; ll++) {
		rndDistPtrVec[ll] = new Uniform_rnd_dist(lowerBoundVec[ll],
				upperBoundVec[ll]);
	}
}

/* ***      COPY AND ASSIGNMEM	***/
Uniform_vec_rnd_dist::Uniform_vec_rnd_dist(
		const Uniform_vec_rnd_dist __attribute__((unused)) & original) :
		Vec_rnd_dist() {
}

const Uniform_vec_rnd_dist&
Uniform_vec_rnd_dist::operator=(const Uniform_vec_rnd_dist & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Uniform_vec_rnd_dist::~Uniform_vec_rnd_dist() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 *//*
 void
 Uniform_vec_rnd_dist::() const
 {
 }
 */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenMObj version 0.4. */
