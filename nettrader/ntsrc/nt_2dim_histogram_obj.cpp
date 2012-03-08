/**\file nt_2dim_histogram_obj.cpp - NT_2dim_histogram_o class implementation
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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/* $Id: nt_2dim_histogram_obj.cpp,v 1.3 2002/03/04 18:09:10 face Exp $
* $Log: nt_2dim_histogram_obj.cpp,v $
* Revision 1.3  2002/03/04 18:09:10  face
* added checking for NaN and Inf values in find_spikes_in_binary_file-
* BinomialStep: redo loop if after state change the states do not sum up
*
* Revision 1.2  2002/02/26 00:01:01  face
* *** empty log message ***
*
* Revision 1.1  2001/10/12 09:18:38  face
* *** empty log message ***
*

*/
#include "nt_2dim_histogram_obj.h"


/* ***      CONSTRUCTORS	***/
/** Create a NT_2dim_histogram_o */
NT_2dim_histogram_o::NT_2dim_histogram_o(NTsize newNumBinsA, NTreal newMinA, NTreal newMaxA, NTsize newNumBinsB, NTreal newMinB, NTreal newMaxB)
{
    numBinsA = newNumBinsA;
    NT_ASSERT(numBinsA > 0);
    minA = newMinA;
    maxA = newMaxA;
    rangeA = maxA - minA;
    NT_ASSERT(rangeA > 0);

    numBinsB = newNumBinsB;
    NT_ASSERT(numBinsB > 0);
    minB = newMinB;
    maxB = newMaxB;
    rangeB = maxB - minB;
    NT_ASSERT(rangeB > 0);

    numBinned = 0;
    numUnbinned = 0;
    Matrix <NTsize> null(numBinsA, numBinsB);
    table = null;
}


/* ***      COPY AND ASSIGNMENT	***/
NT_2dim_histogram_o::NT_2dim_histogram_o(const NT_2dim_histogram_o __attribute__((__unused__)) & original)
{
// add assignment code here
}

const NT_2dim_histogram_o&
NT_2dim_histogram_o::operator= (const NT_2dim_histogram_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
// add assignment code here
    return *this;
}

/* ***      DESTRUCTOR		***/
NT_2dim_histogram_o::~NT_2dim_histogram_o()
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
    @param		the 2 values to be binned
    @return     0 if all fine, 1 if dim 1 value is out of bounds, 2 if only dim 2 value is out of bounds and not 1
   \warning    unknown
   \bug        unknown
 */
NTsize NT_2dim_histogram_o::BinValue(NTreal valueA, NTreal valueB)
{
    NTsize normValA = NTsize(floor(numBinsA*(valueA-minA)/rangeA));
    NTsize normValB = NTsize(floor(numBinsB*(valueB-minB)/rangeB));
    if ( (normValA < 0) || (normValA >= numBinsA) ) {
        numUnbinned++;
        return 1;
    } else if ( (normValB < 0) || (normValB >= numBinsB) ) {
        numUnbinned++;
        return 2;
    } else {
        table[normValA][normValB] += 1;
//		cout << valueA << "->X="<< normValA << "\t" << valueB << "->Y="<<normValB << " set to " << table[normValA][normValB] << endl;
        numBinned++;
    }

    return 0;
}


/** @short Bins ranges are [n range/numBins, (n+1) range/numBins]
    @param		the 2 values to be binned
    @return     0 if all fine, 1 if dim 1 value is out of bounds, 2 if only dim 2 value is out of bounds and not 1
   \warning    unknown
   \bug        unknown
 */
NTsize NT_2dim_histogram_o::BinValue ( vector <NTreal> valueVec)
{
    NT_ASSERT(valueVec.size() == 2);
    return BinValue(valueVec[0],valueVec[1]);
}

/** Resets counters and histogram */
NTreturn NT_2dim_histogram_o::Reset()
{
    numBinned = 0;
    numUnbinned = 0;

    table = Matrix <NTsize> (numBinsA, numBinsB);

    return NT_SUCCESS;
}

/** No descriptions */
Matrix <NTreal> NT_2dim_histogram_o::PDF() const {
    Matrix <NTreal> tmpMtr(numBinsA, numBinsB, 0.0);
    if (0 == numBinned) {
        cerr << "NT_2dim_histogram_o::PDF - Error : PDF cannot be computed on empty histograms. Returning 0.0 matrix." << endl;
        return tmpMtr;
    }

    NTreal sum = 0;
    for (NTsize lla = 0; lla < numBinsA; lla++) {
        for (NTsize llb = 0; llb < numBinsB; llb++) {
            tmpMtr[lla][llb] = table[lla][llb]/NTreal(numBinned);
            sum += tmpMtr[lla][llb];
        }
    }

    return tmpMtr;
}

/** Compute the joint entropy in bits */
NTreal NT_2dim_histogram_o::ShannonEntropy() const
{
    Matrix <NTreal> tmpMtr = PDF();
    NTreal entropy  = 0;

    for (NTsize lla = 0; lla < numBinsA; lla++) {
        for (NTsize llb = 0; llb < numBinsB; llb++) {
            entropy -= NTComputePLogP(tmpMtr[lla][llb]);
        }
    }

    return entropy/log(2.0);
}

/** No descriptions */
void NT_2dim_histogram_o::Show() const {
    cout << table << endl;
}


/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
