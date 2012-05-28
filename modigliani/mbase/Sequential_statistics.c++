/**\file Sequential_statistics.c++ - Sequential_statistics class implementation
 * by Ahmed Aldo Faisal &copy; created 5.11.2000
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

#include "Sequential_statistics.h"

using namespace mbase;

/* ***      CONSTRUCTORS	***/
/** Create a Sequential_statistics */
Sequential_statistics::Sequential_statistics()
{
    Reset();
}


/* ***      COPY AND ASSIGNMEM	***/
Sequential_statistics::Sequential_statistics(
		const Sequential_statistics & original) :
		Obj()
{
// add assignment code here
    counter = original.counter;
    average = original.average;
    valSquareAverage = original.valSquareAverage;
    variance = original.variance;
    min = original.min;
    max = original.max;
}

const Sequential_statistics&
Sequential_statistics::operator= (const Sequential_statistics & right)
{
    if (this == &right) return (*this); // Gracefully handle self assignment
// add assignment code here
    counter = right.counter;
    average = right.average;
    valSquareAverage = right.valSquareAverage;
    variance = right.variance;
    min = right.min;
    max = right.max;
    return (*this);
}

/* ***      DESTRUCTOR		***/
Sequential_statistics::~Sequential_statistics()
{
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
void
Sequential_statistics::Reset()
{
    counter = 0;
    average = 0.0;
    valSquareAverage = 0.0;
    variance = 0.0;
    min = 1/0.0;//MAXFLOAT;//numeric_limits<Real>::max();
    max = -1/0.0;//MINFLOAT;//numeric_limits<Real>::min();
}

/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
void
Sequential_statistics::Add(Real val)
{
    counter++;

    average = UpdateAverage(average, val, counter);
    valSquareAverage = UpdateAverage(valSquareAverage, val*val, counter);
    variance = valSquareAverage - average*average;

    if (val < min) min = val;
    else if ( val > max ) max = val;
}

/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
std::ostream& operator<< (std::ostream& str, const Sequential_statistics & self)
{
    str << self._counter() << "\t" << self._average() <<  "\t" <<self._valSquareAverage()  <<"\t" << self._variance() << "\t" << self._min() <<  "\t" << self._max() ;
    return (str);
}


/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */

Real
Sequential_statistics::UpdateAverage(Real avg, Real val, Size_t n) const
{
    return ( val * (1.0/n) + avg * (1.0 - 1.0/n)	);
}
