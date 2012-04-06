/* nt_vector_obj.cc - real vector class */
/* by Ahmed A. Faisal, 22. 5. 1998(c) */


/*
 * NetTrader - finance management, analysis and simulation system
 * Version:  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
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


/* $Id: nt_vector_obj.cpp,v 1.2 2003/06/23 10:06:52 face Exp $
 * $Log: nt_vector_obj.cpp,v $
 * Revision 1.2  2003/06/23 10:06:52  face
 * Upgraded to gcc-3.3 compilability
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/06/13 20:10:28  face
 * *** empty log message ***
 *
 * Revision 1.6  2000/05/11 11:32:46  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.5  2000/04/28 16:02:43  face
 * found nasty hidden bug, by initialisation of an hideen ellipsoid making the
 * entire program crash
 * on destructor call... reason unknown
 *
 * Revision 1.4  2000/04/27 23:52:45  face
 * IMPLEMENT nt_vector_o DESTRUCTOR
 *
 * Revision 1.3  2000/04/27 18:20:50  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.2  1999/07/05 00:51:28  face
 * added const to paramters !
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.7  1999/02/17 17:46:18  face
 * *** empty log message ***
 *
 * Revision 1.6  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.5  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/11 02:26:40  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/07 23:53:04  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/07 12:56:15  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/12/27 11:53:40  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/16 14:57:08  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.9  1998/11/04 12:58:53  face
 * *** empty log message ***
 *
 * Revision 1.8  1998/10/21 23:55:07  face
 * *** empty log message ***
 *
 * Revision 1.7  1998/10/20 22:32:10  face
 * *** empty log message ***
 *
 * Revision 1.6  1998/10/20 14:51:45  face
 * *** empty log message ***
 *
 * Revision 1.5  1998/10/20 10:18:55  facee
 * *** empty log message ***
 *
 * Revision 1.4  1998/10/16 11:29:32  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/09/28 00:15:27  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/09/24 18:51:08  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/07/07 12:10:10  face
 * Initial revision
 *
 *
 */


#include "nt_vector_obj.h"


#include "memory.h"

//	Method name : Init
//
//	Description : set dimension and allocate memory
//	Input :
//	Output :
//      NOTE : use only on fresh objects or if elem was already freed
//
void
NT_vector_o::Init(NTuint newDim)
{
    dimension = newDim;
    elem = (NTreal*) new NTreal[dimension];
    for (NTuint ll=0; ll < dimension; ll++) elem[ll] = 0.0;
}


//
//	Method name : NT_vector_o
//
//	Description : CONSTRUCTORS
//	Input :
//	Output :
//
NT_vector_o::NT_vector_o()
{
    Init(1);
}

NT_vector_o::NT_vector_o(NTuint newDim)
{
    Init(newDim);
}

NT_vector_o::NT_vector_o(NTuint dim, NTreal * elemVec) {
    Init(dim);
    for (NTuint i = 0; i < dimension; i++) elem[i] = elemVec[i];
}

NT_vector_o::NT_vector_o(NTuint dim, float * elemVec) {
    Init(dim);
    for (NTuint i = 0; i < dimension; i++) elem[i] = elemVec[i];
}


/* if NTreal != double uncomment the below lines
NT_vector_o::NT_vector_o(NTuint dim, double * elemVec){
  Init(dim);
  for (NTuint i = 0; i < dimension; i++) elem[i] = elemVec[i];
}
*/

NT_vector_o::NT_vector_o(NT_vector3_o newVec3)
{
    Init(3);
    elem[0] = newVec3.x;
    elem[1] = newVec3.y;
    elem[2] = newVec3.z;
}

NT_vector_o::NT_vector_o(NTreal a, NTreal b)
{
    Init(2);
    elem[0] = a;
    elem[1] = b;
}


NT_vector_o::NT_vector_o(NTreal a, NTreal b, NTreal c)
{
    Init(3);
    elem[0] = a;
    elem[1] = b;
    elem[2] = c;
}

NT_vector_o::NT_vector_o(NTreal a, NTreal b, NTreal c, NTreal d) {
    Init(4);
    elem[0] = a;
    elem[1] = b;
    elem[2] = c;
    elem[3] = d;
}

NT_vector_o::NT_vector_o(const NT_vector_o &right):
		NT_o()
{
    dimension = right.dimension;
    elem = (NTreal*) new NTreal[dimension];
    for (NTuint i = 0; i < dimension; i++) elem[i] = right.elem[i];
}

//	Method name : ~NT_vector_o
//
//	Description : DESTRUCTOR
//	Input :
//	Output :
//  \Bug Always a source of annoying crashes with delete([]) and gcc 2.95 (linux)
//       commenting it out often helps.
NT_vector_o::~NT_vector_o()
{

    //cerr << "[" << dimension << flush;
    if (dimension < 2) delete elem;
    else delete[] elem;
    //cerr << "]" << endl;

}



NT_vector_o::operator NT_vector3_o() const
{
    NT_vector3_o tmp;
    if  (dimension == 3) {
        tmp.x = elem[0];
        tmp.y = elem[1];
        tmp.z = elem[2];
    } else if (dimension == 2) {
        cerr <<
             "NT_vector3_o(const NT_vector_o NT_SELF) - warning : call with NT_SELF of dimension="
             << _dimension() << " < 3. Copied components, and set missing ones to 0.0."<< endl;
        tmp.x = elem[0];
        tmp.y = elem[1];
        tmp.z = 0.0;
    } else if (dimension == 1) {
        cerr <<
             "NT_vector3_o(const NT_vector_o NT_SELF) - warning : call with NT_SELF of dimension="
             << _dimension() << " < 3. Copied components, and set missing ones to 0.0."<< endl;
        tmp.x = elem[0];
        tmp.y = 0.0;
        tmp.z = 0.0;
    } else {
        cerr <<
             "NT_vector3_o(const NT_vector_o NT_SELF) - warning : call with NT_SELF of dimension="
             << _dimension() << " > 3. Copied first three compnonents."<< endl;
        tmp.x = elem[0];
        tmp.y = elem[1];
        tmp.z = elem[2];
    }
    return tmp;
}


// Access vector components 1..dimension slower and safer
NTreal &
NT_vector_o::operator()(NTint component) const
{
    NT_ASSERT_BOUNDS(component, 1, dimension);
    return elem[component-1];
}


// Access vector components 0..(dimension-1)
NTreal &
NT_vector_o::operator[](NTint component) const
{
    return elem[component];
}

// Arithmetics
NT_vector_o
NT_vector_o::operator+(const NT_vector_o &right) const
{
    NT_vector_o left(*this);
    for (NTuint i = 0; i < dimension; i++)
        left.elem[i] = left.elem[i] + right.elem[i];
    return left;
}

NT_vector_o
NT_vector_o::operator-(const NT_vector_o &right) const
{
    NT_vector_o left(*this);
    for (NTuint i = 0; i < dimension; i++)
    {
        left.elem[i] = left.elem[i] - right.elem[i];
    }
    return left;
}


NTreal
NT_vector_o::operator*(const NT_vector_o &right) const
{
    NTreal result = 0.0;
    for (NTuint i = 0; i < dimension; i++)
    {
        result += elem[i] * right[i];
    }
    return result;
}

// scalar multiplication from the RIGHT side
NT_vector_o
NT_vector_o::operator*(NTreal alpha) const
{
    NT_vector_o left(*this);
    for (NTuint i = 0; i < dimension; i++)
    {
        left[i] = alpha * elem[i];
    }
    return left;
}


NT_vector_o &
NT_vector_o::operator=(NT_vector_o const &right)
{
    NT_ASSERT_DIM( dimension, right.dimension );
    if (this == &right) return *this;
    else
    {
        if (dimension == 1) delete elem;
        else delete[] elem;
        dimension = right.dimension;
        elem = new NTreal[dimension];

        memcpy(elem,right.elem,dimension * sizeof(NTreal));

        return *this;
    }
}



/** Angle between the (dimension)-unit bases and the vector
  @return
  NT_vector_o[dimension] each component contains the angle between the
  vector and the corresponding base vector */
NT_vector_o
NT_vector_o::BaseVectorAngle() const
{
    NT_vector_o oDirection(dimension);
    for (NTuint ll=0; ll < dimension; ll++) {
        NT_vector_o oUnit(dimension);
        oUnit[ll]=1.0;
        oDirection[ll] = oUnit^(*this);
    }
    return oDirection;
}



NTreal
NT_vector_o::LenSqr() const
{
    NTreal len=0.0;
    for (NTuint ll=0;ll<dimension;ll++) len += elem[ll]*elem[ll];
    return len;
}

/** return pointer to element vector.
  \warning The constant pointer to NTreal[dimension] should
  not be modified by any reason.
  */
const NTreal*
NT_vector_o::_elem() const
{
    return (const NTreal*) elem;
}


void
NT_vector_o::set_elem(const NTreal * elemVec)
{
    for (NTuint i = 0; i < dimension; i++) elem[i] = elemVec[i];
}


void
NT_vector_o::set_dimension(NTuint newDim)
{
    // save old entries and create new vector components
    NT_vector_o tmpVec = *this;
    delete elem;
    elem = new NTreal[newDim];

    // copy old entries into vector forall i < newDim
    for (NTuint i = 0; i < newDim; i++) elem[i] = tmpVec[i];
    // fill up empy components i=oldDim..newDim with 0.0
    if (dimension < newDim)
    {
        for (NTuint ll = newDim; ll < dimension; ll++) elem[ll] = 0.0;
    }
    // set new dimension of vector
    dimension = newDim;

}


ostream & operator<<(ostream & os, const NT_vector_o & self)
{
    os << "NT_vector_o"  << NT_SEPARATOR << self._dimension();
    for (NTuint ll=0; ll < self._dimension(); ll++) {
        os << NT_SEPARATOR << self[ll];
    }
    os << endl;
    return os;
}



NTreal
NT_vector_o::operator^(NT_vector_o right)
{
    return CosAng(right);
}



