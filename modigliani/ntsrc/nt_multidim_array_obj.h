/**\file nt_multidim_array_obj.h - NT_multidim_array_o class header
 * by Ahmed Aldo Faisal &copy; created 28.6.2001
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


/* $Id: nt_multidim_array_obj.h,v 1.5 2001/10/12 09:18:39 face Exp $
* $Log: nt_multidim_array_obj.h,v $
* Revision 1.5  2001/10/12 09:18:39  face
* *** empty log message ***
*
* Revision 1.4  2001/10/03 14:37:13  face
* *** empty log message ***
*
* Revision 1.3  2001/06/29 14:38:25  face
* *** empty log message ***
*
* Revision 1.2  2001/06/29 14:17:52  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 13:14:45  face
* adding vraious addtions
* mainly in ntsrc (multidim_array)
*

*/
#ifndef _nt_multidim_array_obj_h_
#define _nt_multidim_array_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* NT includes */
#include "nt_error_obj.h"

/* other includes */
#include <vector>
#include <cstdarg>

/** @short NT_multidim_array_o class
Implements a regular - multidimensional, i.e.
a "dim"-diemsional hypercube with "num" elements per dimension.
\bug unknown
\warning unknown
*/
template <class T> // with regards to Modula-3 :)
class NT_multidim_array_o : public NT_o {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NT_multidim_array_o(NTsize d, NTsize n)
    {
        NT_ASSERT(d > 0);
        dim = d;
        NT_ASSERT(n > 0);
        num = n;
        // implementation range checking
        /* 2DO not working as numeric_limits not implemented in gcc 2.95
        NTreal numElem = pow(n,d);
        NTreal maxIndexRange = numeric_limits<NTsize>::max();
        if (numElem >= maxIndexRange) {
        	cerr <<
        	"NT_multidim_array_o::NT_multidim_array_o - Error : array will contain to many elements"
        	<< numElem << " for supported implementation range "
        	<< maxIndexRange << " ("<< numeric_limits<NTsize>.max()
        	<< "). Undefined behaviour may result." << endl;
        }
        */
        // cache setup
        powerSeriesCacheVec.resize(dim+1);
        // = {1,num,num^2,...,num^dim}
        powerSeriesCacheVec[0] = 1;
        for (NTsize ll = 1; ll < dim+1; ll++) {
            powerSeriesCacheVec[ll] = num * powerSeriesCacheVec[ll-1];
            cerr <<powerSeriesCacheVec[ll] << endl;
        }
        // memory allocation
        try {
            dataVec.resize(powerSeriesCacheVec[dim]);
        }
        catch (bad_alloc& ba) {
            cerr << "NT_multidim_array_o::NT_multidim_array_o - Error : Memory exhausted by allocation of multidimensional array."
                 << ">" << powerSeriesCacheVec[dim] * sizeof(T) << " bytes of memory are necessary for this object."
                 << endl;
        }
    }

    /* ***      COPY AND ASSIGNMENT	***/
    /** copy constructor currently not to be implemented. */
    NT_multidim_array_o(const NT_multidim_array_o & original)
    {
        cerr <<"NT_multidim_array_o::(const NT_multidim_array_o) - Error : Not implemented. Not to be supported ?" << endl;
    }

    /** assignment operator currently not to be implemented. */
    const NT_multidim_array_o&
    operator= (const NT_multidim_array_o & right)
    {
        if (this == &right) return (*this); // Gracefully handle self assignment
        cerr <<"NT_multidim_array_o::operator= - Error : Not implemented. Not to be supported ?" << endl;
        return (*this);
    }

    /* ***      DESTRUCTOR		***/
    virtual ~NT_multidim_array_o()
    {
    }

    /* ***  PUBLIC                                    ***   */
    /** @short	access element at given coordinate position
        @param      none
        @return     none
       \warning    no range checking done on the coordinates
       \bug        unknown
     */
    T &
    Elem(const vector<NTsize> & coordinateVec)
    {
        NTsize hash = Hash(coordinateVec);
        NT_ASSERT (dataVec.size() >= hash);
        return dataVec[ hash ];
    }

    /** @short	access a element with a "meaningless" index
        @param     An index that is guaranteed to touch all elements
        			but has no assigned meaning as to the position of
        			element in relation to others. However
        			index $\in [0,num^dim]$ is guaranteed and will touch
        			all elements ones.
        @return     none
       \warning    Arbitrary index, no information inferable from the index,
       				might change anytime (only range remains guaranteed).
       \bug        unknown
     */
    const T &
    ElemByIndex(NTsize index) const
    {
        NT_ASSERT((index < TotalNumElem()) );
        return dataVec[index];
    }


    void
    SetAll( const T & val )
    {
        for (NTsize ll = 0; ll < TotalNumElem(); ll++) {
            dataVec[ll] = val;
        }
    }

    /** @short	access element at given coordinate position
        @param      none
        @return     none
       \warning    no range checking done on the coordinates
       \bug        unknown
     */
    T &
    Elem(NTsize index1 ...)
    {
        vector <NTsize> coorVec(dim);
        va_list args;
        va_start(args,index1);
        for (NTsize ld=0; ld < dim; ld++) {
            coorVec[ld] = va_arg(args, NTsize);
        }
        va_end(args);
        return Elem(coorVec);
    }


    NTsize
    TotalNumElem() const
    {
        return powerSeriesCacheVec[dim];
    }

//const T & Elem(vector<NTsize> coordinateVec) const;
//const T & Elem(NTsize index1 ...) const;
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    /** Generate an address out of the coordinates of the element
    in the multimensional array. The hash address is the
    decimal representation of the num-ary dim-digit coordinate "number".
    	\warning range of coordinates values (0..num-1) is not tested
     */
    NTsize Hash(const vector <NTsize> & coorVec) const
    {
        NT_ASSERT( coorVec.size() == dim );
        NTsize addressIndex = 0;
        for (NTsize ld = 0; ld < dim; ld++) { //2DO  WHY was ld < num+1 ?!?!?!
            addressIndex += coorVec[ld] * powerSeriesCacheVec[ld];
        }
        return addressIndex;
    }
    /* ***  Data                 ***/
    vector <NTsize> dataVec;
    vector <NTsize> powerSeriesCacheVec;
    NTsize dim;
    NTsize num; // number of elements per dimension

};



/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


#endif /* _nt_multidim_array_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
