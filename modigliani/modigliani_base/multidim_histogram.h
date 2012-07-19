/**
 * @file multidim_histogram.h
 * Multidim_histogram class header
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

#ifndef _mbase_multidim_histogram_h_
#define _mbase_multidim_histogram_h_

/* M core includes */
#include "obj.h"
/* M includes */
#include "multidim_array.h"
#include "aux_math_func.h"
/* other includes */

namespace modigliani_base{

/** @short M_multidim_histogram_o class
\bug unknown
\warning unknown
*/
class Multidim_histogram : public Obj {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Multidim_histogram(Real low, Real high, Size binsPerDim, Size dims);
    Multidim_histogram(const Multidim_histogram & original);
    const Multidim_histogram & operator= (const Multidim_histogram & right);
    virtual ~Multidim_histogram();
    /* ***  Methods              ***/
    /** Enter data into histogram with data $\in [min,max] $*/
    Size BinValue( const std::vector <Real>  & valueVec);
    /** No descriptions */
    Real ShannonEntropy() const;
    /**  Set histogram to initial values */
    void Reset();
    Size _numBinned() const {
        return (numBinned);
    }
    Real _min() const {
        return (min);
    }
    Real _max() const {
        return (max);
    }
    Real _range() const {
        return (range);
    }
    Real _numBinPerDim() const {
        return (numBins);
    }
    Real _dim() const {
        return (dim);
    }
    std::vector <Size> _outsideBinVec() const {
        return (outsideBinVec);
    }
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    Multidim_array<Size> oCube;
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    Size numBinned;
    std::vector <Size> outsideBinVec;
    Real min;
    Real max;
    Real range;
    Size numBins;
    Size dim;
    /* ***  Data                 ***/
};
}
#endif /* _mbase_multidim_histogram_h_ */


/* File skeleton generated by GenMObj version 0.7. */