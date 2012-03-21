/**\file nt_multidim_histogram_obj.cpp - NT_multidim_histogram_o class implementation
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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/* $Id: nt_multidim_histogram_obj.cpp,v 1.4 2002/03/04 18:09:10 face Exp $
* $Log: nt_multidim_histogram_obj.cpp,v $
* Revision 1.4  2002/03/04 18:09:10  face
* added checking for NaN and Inf values in find_spikes_in_binary_file-
* BinomialStep: redo loop if after state change the states do not sum up
*
* Revision 1.3  2001/10/12 09:18:39  face
* *** empty log message ***
*
* Revision 1.2  2001/10/03 14:37:13  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 14:38:25  face
* *** empty log message ***
*

*/
#include "nt_multidim_histogram_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NT_multidim_histogram_o */
NT_multidim_histogram_o::NT_multidim_histogram_o(NTreal low, NTreal high, NTsize bins, NTsize dims)
        :
        oCube(dims,bins)
{
    /* should have been already tested by oCube constructor
    NT_ASSERT( bins > 0);
    NT_ASSERT( dims > 0);
    */
    min = low;
    max = high;
    NT_ASSERT( min < max );
    range = abs(min) + abs(max);
    numBins = bins;
    dim = dims;
    outsideBinVec.resize(dim);
    Reset();
}

/* ***      COPY AND ASSIGNMENT	***/
/*
NT_multidim_histogram_o::NT_multidim_histogram_o(const NT_multidim_histogram_o & original)
{
	 // add assignment code here
	cerr <<"NT_multidim_histogram_o::(const NT_multidim_histogram_o) - Error : Not implemented. Not to be supported ?" << endl;

}
*/

const NT_multidim_histogram_o&
NT_multidim_histogram_o::operator= (const NT_multidim_histogram_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
    // add assignment code here
    cerr <<"NT_multidim_histogram_o::operator= - Error : Not implemented. Not to be supported ?" << endl;
    return *this;
}

/* ***      DESTRUCTOR		***/
NT_multidim_histogram_o::~NT_multidim_histogram_o()
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
NT_multidim_histogram_o::Reset()
{
    numBinned = 0;
    for (NTsize ll = 0; ll < dim; ll++ ) {
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
NTsize
NT_multidim_histogram_o::BinValue( const vector <NTreal>  & valueVec)
{
    NT_ASSERT( valueVec.size() == dim );

    vector <NTsize> coorVec(dim);
    NTsize coor;

    for (NTsize ld = 0; ld < dim; ld++) {
        coor = NTsize(numBins*(valueVec[ld] - min)/range);
        if (coor < numBins) {
            coorVec[ld] = coor;
        } else {
            outsideBinVec[ld] += 1;
            return ld;
        }
    }

    (oCube.Elem(coorVec)) += 1;

    numBinned++;

    return 0;
}


NTreal NT_multidim_histogram_o::ShannonEntropy() const
{
    NTreal tmp = 0;
    NTreal sum = 0;
    for ( NTsize ll = 0; ll < oCube.TotalNumElem(); ll++ ) {
        tmp = oCube.ElemByIndex(ll);
        sum -= NTComputePLogP(tmp);
    }
    return sum/(log(2.0)*oCube.TotalNumElem());
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
/** No descriptions */
