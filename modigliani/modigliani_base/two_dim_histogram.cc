/**
 * @file two_dim_histogram.cc
 * Two_dim_histogram class implementation
 * @author Ahmed Aldo Faisal &copy; created 11.10.2001
 * NetTrader - visualisation, scientific and financial analysis and simulation system
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

#include "two_dim_histogram.h"

using namespace modigliani_base;

/* ***      CONSTRUCTORS    ***/
/** Create a Two_dim_histogram */
Two_dim_histogram::Two_dim_histogram(Size newNumBinsA, Real newMinA, Real newMaxA, Size newNumBinsB, Real newMinB, Real newMaxB)
{
    numBinsA = newNumBinsA;
    M_ASSERT(numBinsA > 0);
    minA = newMinA;
    maxA = newMaxA;
    rangeA = maxA - minA;
    M_ASSERT(rangeA > 0);
    numBinsB = newNumBinsB;
    M_ASSERT(numBinsB > 0);
    minB = newMinB;
    maxB = newMaxB;
    rangeB = maxB - minB;
    M_ASSERT(rangeB > 0);

    numBinned = 0;
    numUnbinned = 0;
    Matrix <Size> null(numBinsA, numBinsB);
    table = null;
}


/* ***      COPY AND ASSIGNMEM    ***/
Two_dim_histogram::Two_dim_histogram(const Two_dim_histogram __attribute__((__unused__)) & original):
        Obj()
{
// add assignment code here
}

const Two_dim_histogram&
Two_dim_histogram::operator= (const Two_dim_histogram & right)
{
    if (this == &right) return (*this); // Gracefully handle self assignment
// add assignment code here
    return (*this);
}

/* ***      DESTRUCTOR        ***/
Two_dim_histogram::~Two_dim_histogram()
{
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */


/** @short Bins ranges are [n range/numBins, (n+1) range/numBins]
    @param        the 2 values to be binned
    @return     0 if all fine, 1 if dim 1 value is out of bounds, 2 if only dim 2 value is out of bounds and not 1
   \warning    unknown
   \bug        unknown
 */
Size Two_dim_histogram::BinValue(Real valueA, Real valueB)
{
    Size normValA = Size(floor(numBinsA*(valueA-minA)/rangeA));
    Size normValB = Size(floor(numBinsB*(valueB-minB)/rangeB));
    if (normValA >= numBinsA) {
        numUnbinned++;
        return (1);
    } else if (normValB >= numBinsB) {
        numUnbinned++;
        return (2);
    } else {
        table[normValA][normValB] += 1;
//        cout << valueA << "->X="<< normValA << "\t" << valueB << "->Y="<<normValB << " set to " << table[normValA][normValB] << endl;
        numBinned++;
    }

    return (0);
}


/** @short Bins ranges are [n range/numBins, (n+1) range/numBins]
    @param        the 2 values to be binned
    @return     0 if all fine, 1 if dim 1 value is out of bounds, 2 if only dim 2 value is out of bounds and not 1
   \warning    unknown
   \bug        unknown
 */
Size Two_dim_histogram::BinValue ( std::vector <Real> valueVec)
{
    M_ASSERT(valueVec.size() == 2);
    return (BinValue(valueVec[0],valueVec[1]));
}

/** Resets counters and histogram */
ReturnEnum Two_dim_histogram::Reset()
{
    numBinned = 0;
    numUnbinned = 0;

    table = Matrix <Size> (numBinsA, numBinsB);

    return (ReturnEnum::SUCCESS);
}

/** No descriptions */
Matrix <Real> Two_dim_histogram::PDF() const {
    Matrix <Real> tmpMtr(numBinsA, numBinsB, 0.0);
    if (0 == numBinned) {
        std::cerr << "Two_dim_histogram::PDF - Error : PDF cannot be computed on empty histograms. Returning 0.0 matrix." << std::endl;
        return (tmpMtr);
    }

    Real sum = 0;
    for (Size lla = 0; lla < numBinsA; lla++) {
        for (Size llb = 0; llb < numBinsB; llb++) {
            tmpMtr[lla][llb] = table[lla][llb]/Real(numBinned);
            sum += tmpMtr[lla][llb];
        }
    }

    return (tmpMtr);
}

/** Compute the joint entropy in bits */
Real Two_dim_histogram::ShannonEntropy() const
{
    Matrix <Real> tmpMtr = PDF();
    Real entropy  = 0;

    for (Size lla = 0; lla < numBinsA; lla++) {
        for (Size llb = 0; llb < numBinsB; llb++) {
            entropy -= MComputePLogP(tmpMtr[lla][llb]);
        }
    }

    return (entropy/log(2.0));
}

/** No descriptions */
void Two_dim_histogram::Show() const {
    std::cout << table << std::endl;
}


/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenMObj version 0.7. */