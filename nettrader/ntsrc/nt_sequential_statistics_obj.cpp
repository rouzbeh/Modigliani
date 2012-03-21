/**\file nt_sequential_statistics_obj.cpp - NT_sequential_statistics_o class implementation
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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/* $Id: nt_sequential_statistics_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
* $Log: nt_sequential_statistics_obj.cpp,v $
* Revision 1.1  2001/06/11 13:18:56  face
* *** empty log message ***
*

*/
#include "nt_sequential_statistics_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NT_sequential_statistics_o */
NT_sequential_statistics_o::NT_sequential_statistics_o()
{
    Reset();
}


/* ***      COPY AND ASSIGNMENT	***/
NT_sequential_statistics_o::NT_sequential_statistics_o(
		const NT_sequential_statistics_o & original) :
		NT_o()
{
// add assignment code here
    counter = original.counter;
    average = original.average;
    valSquareAverage = original.valSquareAverage;
    variance = original.variance;
    min = original.min;
    max = original.max;
}

const NT_sequential_statistics_o&
NT_sequential_statistics_o::operator= (const NT_sequential_statistics_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
// add assignment code here
    counter = right.counter;
    average = right.average;
    valSquareAverage = right.valSquareAverage;
    variance = right.variance;
    min = right.min;
    max = right.max;
    return *this;
}

/* ***      DESTRUCTOR		***/
NT_sequential_statistics_o::~NT_sequential_statistics_o()
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
NT_sequential_statistics_o::Reset()
{
    counter = 0;
    average = 0.0;
    valSquareAverage = 0.0;
    variance = 0.0;
    min = 1/0.0;//MAXFLOAT;//numeric_limits<NTreal>::max();
    max = -1/0.0;//MINFLOAT;//numeric_limits<NTreal>::min();
}

/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
void
NT_sequential_statistics_o::Add(NTreal val)
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
ostream& operator<< (ostream& str, const NT_sequential_statistics_o & self)
{
    str << self._counter() << "\t" << self._average() <<  "\t" <<self._valSquareAverage()  <<"\t" << self._variance() << "\t" << self._min() <<  "\t" << self._max() ;
    return str;
}


/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */

NTreal
NT_sequential_statistics_o::UpdateAverage(NTreal avg, NTreal val, NTsize n) const
{
    return ( val * (1.0/n) + avg * (1.0 - 1.0/n)	);
}


/* File skeleton generated by GenNTObj version 0.7. */
