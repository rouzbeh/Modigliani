/**\file Histogram.h - Histogram class header
 * by Ahmed Aldo Faisal &copy; created 11.10.2000
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

#ifndef _mbase_histogram_h_
#define _mbase_histogram_h_

/* M core includes */
#include "main.h"
#include "types.h"
#include "Obj.h"
/* M includes */
#include "Error.h"

/* other includes */
#include <vector>
#include <cmath>
#include <iostream>

/** @short Histogram class
 \bug unknown
 \warning unknown
 */
namespace mbase {
typedef std::vector<Msize> HistogramBins;
class Histogram: public Obj {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Histogram(Mreal minVal = 0, Mreal maxVal = 1, Msize numberOfBins =
			10);
	Histogram(const Histogram & original);
	const Histogram & operator=(const Histogram & right);
	virtual ~Histogram();
	/* ***  Methods              ***/
	void Reset();
	void BinValue(Mreal value);
	void Show(bool showHeader = false) const;
	std::vector<Mreal> PDF() const;
	/* in bits */
	Mreal ShannonEntropy() const;

	friend std::ostream& operator<<(std::ostream& str, const Histogram & self);
	/* ***  Data                 ***/
	HistogramBins bins;
	Mreal numBins;
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	Mreal min;
	Mreal max;
	Mreal binWidth;
	Mreal numDataPoints;
	Msize numOutliers;
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/

};
}
#endif /* _mbase_histogram_h_ */

/* File skeleton generated by GenMObj version 0.7. */
