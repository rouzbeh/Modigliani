/* nt_vector_obj.h - real vector class */
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

/* $Id: nt_vector_obj.h,v 1.2 2003/06/20 13:26:00 face Exp $
 * $Log: nt_vector_obj.h,v $
 * Revision 1.2  2003/06/20 13:26:00  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.5  2000/06/13 20:10:28  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/05/11 11:32:46  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.3  1999/07/05 00:51:29  face
 * added const to paramters !
 *
 * Revision 1.2  1999/07/04 03:17:43  face
 * minor mods
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
 * Revision 1.11  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.10  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.9  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.11  1999/01/07 12:56:23  face
 * *** empty log message ***
 *
 * Revision 1.10  1998/12/27 11:53:34  face
 * *** empty log message ***
 *
 * Revision 1.9  1998/12/25 21:28:17  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.8  1998/12/16 14:57:14  face
 * *** empty log message ***
 *
 * Revision 1.7  1998/10/21 23:55:11  face
 * *** empty log message ***
 *
 * Revision 1.6  1998/10/20 22:32:14  face
 * *** empty log message ***
 *
 * Revision 1.5  1998/10/20 14:51:52  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/10/19 14:09:03  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/10/16 11:29:24  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/09/24 18:51:11  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/07/07 12:10:15  face
 * Initial revision
 *
 *
 */

#ifndef _nt_vector_obj_h_
#define _nt_vector_obj_h_

#include "nt_obj.h"

#include "nt_vector3_obj.h"

#include <iostream>
#include <math.h>

//
//	Class name : NT_vector_o
//
/**	A n-dimensional real vector class with visualization capabilities
 */
class NT_vector_o : public NT_o {
public:
    NT_vector_o();
    NT_vector_o(NTuint dim);
    NT_vector_o(NTuint dim, NTreal * elemVec);
    NT_vector_o(NTuint dim, float * elemVec);
    // if NTreal != double uncomment the below line
    // NT_vector_o(NTuint dim, double * elemVec);
    NT_vector_o(NT_vector3_o newVector3);
    NT_vector_o(NTreal a, NTreal b);
    NT_vector_o(NTreal a, NTreal b, NTreal c);
    NT_vector_o(NTreal a, NTreal b, NTreal c, NTreal d);
    NT_vector_o(const NT_vector_o &);
    NT_vector_o& operator=(NT_vector_o const &);
    ~NT_vector_o();
    /*		Methods			*/
    /*            Operators		*/
    //! addition C = A+B
    NT_vector_o operator+(const NT_vector_o &) const;
    //! subtraction C = A-B
    NT_vector_o operator-(const NT_vector_o &) const;
    //! scalar product r = A*B
    NTreal operator*(const NT_vector_o &) const;
    //! scalar (dot) product r = A*B
    NTreal Dot(const NT_vector_o right) const
    {
        return (*this)*(right);
    }
    /** cosine of the angle between two NT_vector_o */
    NTreal CosAng( const NT_vector_o right)
    {
        return (*this)*(right) / (Length() * right.Length());
    }
    //! Scaling, factor multiplication (from the RIGHT) C = A*alpha
    NT_vector_o operator*(NTreal) const;
    //! Access vector components 1..dimension slower and safer
    NTreal &operator()(NTint component) const;
    //! Access vector components 0..(dimension-1) fast and no range checking
    NTreal &operator[](NTint component) const;
    NTreal operator^(NT_vector_o b);
    operator NT_vector3_o() const;
    friend ostream & operator<<(ostream & os, const NT_vector_o & self);
    /*		Functions		*/
    NT_vector_o BaseVectorAngle() const;
    inline NTreal Length() const {
        return sqrt(LenSqr());
    };
    NTreal LenSqr() const;
    NTuint _dim() const {
        return dimension;
    }
    /* 		Element Functions	*/
    NTuint _dimension() const {
        return dimension;
    };
    const NTreal* _elem() const;
    void set_elem(const NTreal * elemVec);
    /*		Data		*/
private:
    /*		Methods		*/
    void Init(NTuint newDim);//!< call only on first Init, or free elem before
    void set_dimension(NTuint newDim);
    /*		Data		*/
    NTreal* elem;
    NTuint dimension;
};





#endif /* _nt_vector_obj_h_ */














