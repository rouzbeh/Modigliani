/**
 * @file two_dim_histogram.h
 * Two_dim_histogram class header
 * @author Ahmed Aldo Faisal &copy; created 11.10.2001
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

#ifndef _modigliani_base_two_dim_histogram_h_
#define _modigliani_base_two_dim_histogram_h_

/* M core includes */
#include "obj.h"
/* M includes */
#include "error.h"
#include "aux_math_func.h"

/* other includes */
#include <vector>
#include <tnt/tnt_cmat.h>

using namespace TNT;
using TNT::Matrix;

namespace modigliani_base {
/** @short M_2dim_histogram_o class
 \bug unknown
 \warning unknown
 */
class Two_dim_histogram : public Obj {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Two_dim_histogram(Size numBinsA, Real minA, Real maxA, Size numBinsB, Real minB, Real maxB);
    Two_dim_histogram(const Two_dim_histogram & original) = delete;
    const Two_dim_histogram & operator= (const Two_dim_histogram & right) = delete;
    virtual ~Two_dim_histogram();
    /* ***  Methods              ***/
    /** No descriptions */
    ReturnEnum Reset();
    /** No descriptions */
    Real ShannonEntropy() const;
    /** No descriptions */
    Size BinValue(Real valueA, Real valueB);
    /** No descriptions */
    void Show() const;
    /** No descriptions */
    Matrix <Real> PDF() const;
    /** No descriptions */
    Size BinValue ( std::vector <Real> valueVec);
    /** No descriptions */
    Size _numBinned() {
        return (numBinned);
    }
    Size _numUnbinned() {
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
    Matrix <Size> table;
    Size numBinsA;
    Real maxA;
    Real minA;
    Real rangeA;
    Size numBinsB;
    Real maxB;
    Real minB;
    Real rangeB;
    Size numBinned;
    Size numUnbinned;
};
}

#endif /* _modigliani_base_two_dim_histogram_h_ */
