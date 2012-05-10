/**\file Two_dim_histogram.h - M_2dim_histogram_o class header
 * by Ahmed Aldo Faisal &copy; created 11.10.2001
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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

#ifndef _mbase_two_dim_histogram_h_
#define _mbase_two_dim_histogram_h_

/* M core includes */
#include "Obj.h"
/* M includes */
#include "Error.h"
#include "aux_math_func.h"

/* other includes */
#include <vector>
#include <tnt/tnt_cmat.h>

using namespace TNT;
using TNT::Matrix;

/** @short M_2dim_histogram_o class
 \bug unknown
 \warning unknown
 */

namespace mbase {
class Two_dim_histogram : public Obj {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Two_dim_histogram(Msize numBinsA, Mreal minA, Mreal maxA, Msize numBinsB, Mreal minB, Mreal maxB);
	Two_dim_histogram(const Two_dim_histogram & original);
	const Two_dim_histogram & operator= (const Two_dim_histogram & right);
	virtual ~Two_dim_histogram();
	/* ***  Methods              ***/
	/** No descriptions */
	Mreturn Reset();
	/** No descriptions */
	Mreal ShannonEntropy() const;
	/** No descriptions */
	Msize BinValue(Mreal valueA, Mreal valueB);
	/** No descriptions */
	void Show() const;
	/** No descriptions */
	Matrix <Mreal> PDF() const;
	/** No descriptions */
	Msize BinValue ( std::vector <Mreal> valueVec);
	/** No descriptions */
	Msize _numBinned() {
		return (numBinned);
	}
	Msize _numUnbinned() {
		return (numUnbinned);
	}
	/** No descriptions */
	/** No descriptions */

	/* ***  Data                 ***/
protected:
	/* ***  Methods
	 ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	Matrix <Msize> table;
	Msize numBinsA;
	Mreal maxA;
	Mreal minA;
	Mreal rangeA;
	Msize numBinsB;
	Mreal maxB;
	Mreal minB;
	Mreal rangeB;
	Msize numBinned;
	Msize numUnbinned;
};
}

#endif /* _mbase_two_dim_histogram_h_ */
