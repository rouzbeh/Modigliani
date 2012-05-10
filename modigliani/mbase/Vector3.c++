/** \file Vector3.c++ - Vector3 class implementation
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
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Vector3.h"

using namespace mbase;

/** The following pre-defined vectors are helpfull speeding and
 * coding more complex computations
 * However, there might be a "static initilization order fiasco"
 * as classes using them might be created before these are created.
	const Vector3 M_UnitXVec3(1.0,0.0,0.0);
	const Vector3 M_UnitYVec3(0.0,1.0,0.0);
	const Vector3 M_UnitZVec3(0.0,0.0,1.0);
	const Vector3 M_UnitMinusZVec3(0.0,0.0,-1.0);
	const Vector3 M_UnitXYZVec3(1.0,1.0,1.0);
	const Vector3 M_OriginVec3(0.0,0.0,0.0);
*  to prevent this, the "Construct On First Use Idiom" is used
*  instead of the objects, a function returning the identical
*  object is used, but which prevents the fiasco as the - now -
*  local static is constructed on the first sweep of the control flow
*/

Vector3& M_UnitXVec3()
{
    static Vector3* vec = new Vector3(1.0,0.0,0.0);
    return (*vec);
}

Vector3& M_UnitYVec3()
{
    static Vector3* vec = new Vector3(0.0,1.0,0.0);
    return (*vec);
}

Vector3& M_UnitZVec3()
{
    static Vector3* vec = new Vector3(0.0,0.0,1.0);
    return (*vec);
}

Vector3& M_UnitMinusZVec3()
{
    static Vector3* vec = new Vector3(0.0,0.0,-1.0);
    return (*vec);
}

Vector3& M_UnitXYZVec3()
{
    static Vector3* vec = new Vector3(1.0,1.0,1.0);
    return (*vec);
}

Vector3& M_OriginVec3()
{
    static Vector3* vec = new Vector3(0.0,0.0,0.0);
    return (*vec);
}



Vector3
Vector3::XYZrotation(const Vector3 & rotV) const
{
    Mdouble phi = M_PI * rotV[0]/180.0;
    Mdouble theta = M_PI * rotV[1]/180.0;
    Mdouble psi = M_PI * rotV[2]/180.0;
	Vector3 oResult(
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
    return (oResult);
}

