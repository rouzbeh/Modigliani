/** \file nt_vector3_obj.h - NT_solid_o class declaration
 * by Ahmed Aldo Faisal &copy; created  15.10.1998
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:  0.2
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


/* $Id: nt_vector3_obj.h,v 1.2 2003/06/20 13:26:00 face Exp $
 * $Log: nt_vector3_obj.h,v $
 * Revision 1.2  2003/06/20 13:26:00  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/10/01 09:33:48  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/05/11 11:32:46  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.7  2000/04/27 23:52:45  face
 * IMPLEMENT nt_vector_o DESTRUCTOR
 *
 * Revision 1.6  2000/04/27 18:20:50  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.5  2000/04/26 17:10:38  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.4  1999/07/17 18:22:36  face
 * added NT_UnityMinusZVec3:
 *
 * Revision 1.3  1999/07/16 15:05:41  face
 * corrected error, switched to vector in id:mnanagement.added code::^
 *
 * Revision 1.2  1999/07/05 00:51:27  face
 * added const to paramters !
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.2  1999/02/17 17:46:22  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/18 01:02:26  face
 * Initial revision
 *
 * Revision 1.3  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/11 02:26:45  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/07 23:52:56  face
 * Initial revision
 *
 * Revision 1.1  1998/12/25 21:28:17  face
 * Initial revision
 *
 *
 */

#ifndef _NT_vector3_h_
#define _NT_vector3_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
#include "nt_error_obj.h"

#include "nt_aux_math_func.h"

#include <iostream>
/** @short NT_vector3_o class
Implementation of a three-dimensional
 real-valued vector. Useable it as a 2D vector, too; the third (Z)
 dimension defaults to zero.
\warning All input angles (roationas) in degree!.
\bug unknown
*/


class NT_vector3_o;

extern NT_vector3_o& NT_UnitXVec3();
extern NT_vector3_o& NT_UnitYVec3();
extern NT_vector3_o& NT_UnitZVec3();
extern NT_vector3_o& NT_UnitMinusZVec3();
extern NT_vector3_o& NT_UnitXYZVec3();
extern NT_vector3_o& NT_OriginVec3();


class NT_vector3_o : public NT_o {
public:
    // NT_vector3_o components
    NTreal x,y,z;

    /* ***	  CONSTRUCTORS          ***/
    NT_vector3_o() {
        x=y=z=0;
    }

    NT_vector3_o(NTreal pX, NTreal pY, NTreal pZ)
            :
            x(pX),
            y(pY),
            z(pZ)
    {
    }

    /* ***      COPY AND ASSIGNMENT	***/
    NT_vector3_o(const NT_vector3_o & original)
            :
            NT_o(),
            x(original.x),
            y(original.y),
            z(original.z)
    {
    }

    const NT_vector3_o&
    operator= (const NT_vector3_o & right)
    {
        if (this == &right) return *this; // Gracefully handle self assignment
        // add assignment code here
        x = right.x;
        y = right.y;
        z = right.z;
        return *this;
    }


    /* ***      DESTRUCTOR		***/
    ~NT_vector3_o()
    {
    }


    // NT_vector3_o arithmatic

    NT_vector3_o operator+(const NT_vector3_o & pV) const
    {
        NT_vector3_o out( x+pV.x, y+pV.y, z+pV.z );
        return out;
    }

    NT_vector3_o operator-(const NT_vector3_o & pV) const
    {
        NT_vector3_o out( x-pV.x, y-pV.y, z-pV.z );
        return out;
    }

    NT_vector3_o& operator+=(const NT_vector3_o & pV)
    {
        x+=pV.x;
        y+=pV.y;
        z+=pV.z;
        return *this;
    }

    NT_vector3_o& operator-=(const NT_vector3_o & pV)
    {
        x-=pV.x;
        y-=pV.y;
        z-=pV.z;
        return *this;
    }

    /** cross product with another vector (returns a scalar) */
    NT_vector3_o X(const NT_vector3_o & pV) const
    {
        NT_vector3_o out( y*pV.z-z*pV.y, z*pV.x-x*pV.z, x*pV.y-y*pV.x );
        return out;
    }


    /** Access vector components 1..dimension slower and safer */
    NTreal
    operator()(NTint component) const
    {
        NT_ASSERT_BOUNDS(component, 1, 3);
        switch (component) {
        case 1:
            return x;
            break;
        case 2:
            return y;
            break;
        case 3:
            return z;
            break;
        default:
            return 0.0;
            break;
        }
        return 0.0;
    }


    /** Access vector components read/write */
    NTreal &
    operator[](NTint component)
    {
        switch (component) {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            return x;
            break;
        }
        return x;
    }

    /** Access vector components read-only */
    NTreal
    operator[](NTint component) const
    {
        switch (component) {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        default:
            return x;
            break;
        }
        return 0;
    }

    NT_vector3_o Cross( const NT_vector3_o & pV ) const
    {
        return (*this).X(pV);
    }

    friend NT_vector3_o Cross( const NT_vector3_o & p1, const NT_vector3_o & p2 )
    {
        return p1.X(p2);
    }

    /** dot product with another vector (returns a vector)  */
    NTreal operator*(const NT_vector3_o & pV) const
    {
        return x*pV.x + y*pV.y + z*pV.z;
    }

    NTreal Dot(const NT_vector3_o & pV) const
    {
        return (*this)*(pV);
    }

    friend NTreal Dot( const NT_vector3_o & p1, const NT_vector3_o & p2 )
    {
        return p1 * p2;
    }

    /**  scalar muliplication from the right */
    NT_vector3_o operator*(const NTreal pR) const
    {
        NT_vector3_o out( x*pR, y*pR, z*pR );
        return out;
    }

    /**  scalar muliplication from the left */
    friend NT_vector3_o operator*(const NTreal pR, const NT_vector3_o pV)
    {
        NT_vector3_o out( pV.x*pR, pV.y*pR, pV.z*pR );
        return out;
    }

    /**  scalar muliplication from the right applied to *this */
    NT_vector3_o& operator*=(const NTreal pR)
    {
        x*=pR;
        y*=pR;
        z*=pR;
        return *this;
    }

    /** Return a vector rotated around X, Y and then Z axis.
        The corresponding rotation angles  phi, theta and psi are
        given in degree. */
    NT_vector3_o XYZrotation(const NT_vector3_o & rotV) const;


    /** squared euclidean length of vector */
    NTreal LenSqr(void) const
    {
        return x*x+y*y+z*z;
    }

    /** euclidean length of vector */
    NTreal Length(void) const
    {
        return sqrt(LenSqr());
    }

    /** Normalize vector
    Normlises vector, or unmodified if length is 0
    */
    void Norm()
    {
        NTreal tmpLen = Length();
        if (tmpLen > 0) {
            x /= tmpLen;
            y /= tmpLen;
            z /= tmpLen;
        }
    }

    /** Return A normalized version of vector */
    NT_vector3_o Norm() const
    {
        NTreal tmpLen = Length();
        if (tmpLen > 0) return NT_vector3_o(x/tmpLen, y/tmpLen, z/tmpLen);
        else return NT_OriginVec3();
    }


    /** projection (componeNT of *this parallel to pV)
      Note: componeNT perpendicular to pV is:  *this - Proj(pV) */
    NT_vector3_o Proj(const NT_vector3_o & pV)
    {
        NT_vector3_o out( pV * (this->Dot(pV) / pV.LenSqr()) );
        return out;
    }

    /** cosine of the angle between two NT_vector3_os */
    NTreal CosAng( const NT_vector3_o & pV)
    {
        return this->Dot(pV) / (Length() * pV.Length());
    }


    NTreal
    operator^(const NT_vector3_o & right)
    {
        return NT_SELF.CosAng(right);
    }



    /** Angle between the 3D-unit bases and the vector
      @return
      NT_vector3_o each component contains the angle between the
      vector and the corresponding base vector */
    NT_vector3_o
    BaseVectorAngle() const
    {
        NT_vector3_o oDirection;
        for (NTuint ll=0; ll < 3; ll++) {
            NT_vector3_o oUnit;
            oUnit[ll]=1.0;
            oDirection[ll] = oUnit^(*this);
        }
        return oDirection;
    }



    void
    set_elem(const NTreal * elemVec)
    {
        x = elemVec[0];
        y = elemVec[1];
        z = elemVec[2];
    }

    /** return the dimension, which is 3 */
    NTuint _dimension() const {
        return 3;
    }
    /** return a pointer to a DELETABLE NTreal[3] */
    NTreal* _elem() const {
        NTreal * tmpPtr = new NTreal[3];
        tmpPtr[0] = x;
        tmpPtr[1] = y;
        tmpPtr[2] = z;
        return tmpPtr;
    }

    /** comparison */
    NTbool operator==( const NT_vector3_o & pV) const
    {
        return (x==pV.x && y==pV.y && z==pV.z) ? NT_TRUE: NT_FALSE;
    }

    NTbool operator!=( const NT_vector3_o & pV) const
    {
        return (x!=pV.x || y!=pV.y || z!=pV.z) ? NT_TRUE: NT_FALSE;
    }


    /** pretty print components */
    friend ostream& operator<<(ostream& pStr, const NT_vector3_o& pV)
    {
        return (pStr << '(' << pV.x << ',' << pV.y << ',' << pV.z << ')');
    }

    /** read components from stream pStr */
    friend istream& operator>>(istream pStr, NT_vector3_o& pV)
    {
        return (pStr >> pV.x >> pV.y >> pV.z);
    }
}	;





#endif /* _NT_vector3_h_ */









