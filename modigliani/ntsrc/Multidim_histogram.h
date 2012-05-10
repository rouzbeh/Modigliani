/**\file nt_multidim_histogram.h - M_multidim_histogram_o class header
 * by Ahmed Aldo Faisal &copy; created 29.6.2001
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

#ifndef _mbase_multidim_histogram_h_
#define _mbase_multidim_histogram_h_

/* M core includes */
#include "Obj.h"
/* M includes */
#include "Multidim_array.h"
#include "aux_math_func.h"
/* other includes */

/** @short M_multidim_histogram_o class
\bug unknown
\warning unknown
*/
namespace mbase{
class Multidim_histogram : public Obj {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
	Multidim_histogram(Mreal low, Mreal high, Msize binsPerDim, Msize dims);
    Multidim_histogram(const Multidim_histogram & original);
    const Multidim_histogram & operator= (const Multidim_histogram & right);
    virtual ~Multidim_histogram();
    /* ***  Methods              ***/
    /** Enter data into histogram with data $\in [min,max] $*/
    Msize BinValue( const std::vector <Mreal>  & valueVec);
    /** No descriptions */
    Mreal ShannonEntropy() const;
    /**  Set histogram to initial values */
    void Reset();
    Msize _numBinned() const {
        return (numBinned);
    }
    Mreal _min() const {
        return (min);
    }
    Mreal _max() const {
        return (max);
    }
    Mreal _range() const {
        return (range);
    }
    Mreal _numBinPerDim() const {
        return (numBins);
    }
    Mreal _dim() const {
        return (dim);
    }
    std::vector <Msize> _outsideBinVec() const {
        return (outsideBinVec);
    }
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    Multidim_array<Msize> oCube;
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    Msize numBinned;
    std::vector <Msize> outsideBinVec;
    Mreal min;
    Mreal max;
    Mreal range;
    Msize numBins;
    Msize dim;
    /* ***  Data                 ***/
};
}
#endif /* _mbase_multidim_histogram_h_ */


/* File skeleton generated by GenMObj version 0.7. */
