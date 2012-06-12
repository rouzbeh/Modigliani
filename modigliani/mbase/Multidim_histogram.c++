/**
 * @file Multidim_histogram.c++
 * Multidim_histogram class implementation
 * @author Ahmed Aldo Faisal &copy; created 29.6.2001
 * @version  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * @section LICENSE
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

#include "Multidim_histogram.h"

using namespace mbase;

/* ***      CONSTRUCTORS	***/
/** Create a Multidim_histogram */
Multidim_histogram::Multidim_histogram(Real low, Real high, Size_t bins,
		Size_t dims) :
		oCube(dims, bins) {
	/* should have been already tested by oCube constructor
	 M_ASSERT( bins > 0);
	 M_ASSERT( dims > 0);
	 */
	min = low;
	max = high;
	M_ASSERT(min < max);
	range = abs(min) + abs(max);
	numBins = bins;
	dim = dims;
	outsideBinVec.resize(dim);
	Reset();
}

/* ***      COPY AND ASSIGNMEM	***/
/*
 Multidim_histogram::Multidim_histogram(const Multidim_histogram & original)
 {
 // add assignment code here
 cerr <<"Multidim_histogram::(const Multidim_histogram) - Error : Not implemented. Not to be supported ?" << endl;

 }
 */

const Multidim_histogram&
Multidim_histogram::operator=(const Multidim_histogram & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	std::cerr
			<< "Multidim_histogram::operator= - Error : Not implemented. Not to be supported ?"
			<< std::endl;
	return (*this);
}

/* ***      DESTRUCTOR		***/
Multidim_histogram::~Multidim_histogram() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
void Multidim_histogram::Reset() {
	numBinned = 0;
	for (Size_t ll = 0; ll < dim; ll++) {
		outsideBinVec[ll] = 0;
	}
	oCube.SetAll(0);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
Size_t Multidim_histogram::BinValue(const std::vector<Real> & valueVec) {
	M_ASSERT(valueVec.size() == dim);

	std::vector<Size_t> coorVec(dim);
	Size_t coor;

	for (Size_t ld = 0; ld < dim; ld++) {
		coor = Size_t(numBins * (valueVec[ld] - min) / range);
		if (coor < numBins) {
			coorVec[ld] = coor;
		} else {
			outsideBinVec[ld] += 1;
			return (ld);
		}
	}

	(oCube.Elem(coorVec)) += 1;

	numBinned++;

	return (0);
}

Real Multidim_histogram::ShannonEntropy() const {
	Real tmp = 0;
	Real sum = 0;
	for (Size_t ll = 0; ll < oCube.TotalNumElem(); ll++) {
		tmp = oCube.ElemByIndex(ll);
		sum -= mbase::MComputePLogP(tmp);
	}
	return (sum / (log(2.0) * oCube.TotalNumElem()));
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenMObj version 0.7. */
/** No descriptions */
