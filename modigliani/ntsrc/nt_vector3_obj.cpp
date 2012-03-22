/** \file nt_vector3_obj.cpp - NT_solid_o class implementation
 * by Ahmed Aldo Faisal &copy; created  18.1.1998
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


/* $Id: nt_vector3_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_vector3_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
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
 * Revision 1.4  1999/07/20 01:15:30  face
 * added NT_string_o as string type
 *
 * Revision 1.3  1999/07/17 18:22:35  face
 * added NT_UnityMinusZVec3:
 *
 * Revision 1.2  1999/07/16 15:05:40  face
 * corrected error, switched to vector in id:mnanagement.added code::^
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.3  1999/05/08 23:59:01  face
 * *** empty log message ***
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
 * Revision 1.1  1999/01/18 01:02:26  face
 * Initial revision
 *
 */


#include "nt_vector3_obj.h"

/** The following pre-defined vectors are helpfull speeding and
 * coding more complex computations
 * However, there might be a "static initilization order fiasco"
 * as classes using them might be created before these are created.
	const NT_vector3_o NT_UnitXVec3(1.0,0.0,0.0);
	const NT_vector3_o NT_UnitYVec3(0.0,1.0,0.0);
	const NT_vector3_o NT_UnitZVec3(0.0,0.0,1.0);
	const NT_vector3_o NT_UnitMinusZVec3(0.0,0.0,-1.0);
	const NT_vector3_o NT_UnitXYZVec3(1.0,1.0,1.0);
	const NT_vector3_o NT_OriginVec3(0.0,0.0,0.0);
*  to prevent this, the "Construct On First Use Idiom" is used
*  instead of the objects, a function returning the identical
*  object is used, but which prevents the fiasco as the - now -
*  local static is constructed on the first sweep of the control flow
*/

NT_vector3_o& NT_UnitXVec3()
{
    static NT_vector3_o* vec = new NT_vector3_o(1.0,0.0,0.0);
    return *vec;
}

NT_vector3_o& NT_UnitYVec3()
{
    static NT_vector3_o* vec = new NT_vector3_o(0.0,1.0,0.0);
    return *vec;
}

NT_vector3_o& NT_UnitZVec3()
{
    static NT_vector3_o* vec = new NT_vector3_o(0.0,0.0,1.0);
    return *vec;
}

NT_vector3_o& NT_UnitMinusZVec3()
{
    static NT_vector3_o* vec = new NT_vector3_o(0.0,0.0,-1.0);
    return *vec;
}

NT_vector3_o& NT_UnitXYZVec3()
{
    static NT_vector3_o* vec = new NT_vector3_o(1.0,1.0,1.0);
    return *vec;
}

NT_vector3_o& NT_OriginVec3()
{
    static NT_vector3_o* vec = new NT_vector3_o(0.0,0.0,0.0);
    return *vec;
}



NT_vector3_o
NT_vector3_o::XYZrotation(const NT_vector3_o & rotV) const
{
    NTdouble phi = M_PI * rotV[0]/180.0;
    NTdouble theta = M_PI * rotV[1]/180.0;
    NTdouble psi = M_PI * rotV[2]/180.0;
    NT_vector3_o oResult(
        /* X */
        cos(theta)*cos(psi)*x
        -cos(theta)*sin(psi)*y
        -sin(theta)*z,
        /* Y */
        (-sin(phi)*sin(theta)*cos(psi)+cos(phi)*sin(psi))*x
        +(sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi))*y
        -sin(phi)*cos(theta)*z,
        /* Z */
        (cos(phi)*sin(theta)*cos(psi)
         +sin(phi)*sin(psi))*x+
        (-cos(phi)*sin(theta)*sin(psi)+sin(phi)*cos(psi))*y
        +cos(phi)*cos(theta)*z
    );
    return oResult;
}

