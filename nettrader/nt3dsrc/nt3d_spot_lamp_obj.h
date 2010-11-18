/**\file nt3d_spot_lamp_obj.h - NT3D_spot_lamp_o class header
 * by Ahmed Aldo Faisal &copy; created 4.7.1999
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.4
 * Copyright (C) 1998,199 Ahmed Aldo Faisal
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

/* $Id: nt3d_spot_lamp_obj.h,v 1.1 2001/06/29 13:16:55 face Exp $
* $Log: nt3d_spot_lamp_obj.h,v $
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.5  2000/05/11 12:42:08  face
* removed nt3d_rainbow_o by intergrating it into nt3d_light_o
*
* Revision 1.4  2000/05/11 11:32:44  face
* upgrade to NetTrader Version 0.5.
* Common state of all files checkin (Versioon 0.5)
* rearranged color and material classes...
* texture class added
*
* Revision 1.3  1999/07/17 18:22:07  face
* imodified
*
* Revision 1.2  1999/07/16 15:05:18  face
* added code::^
*
* Revision 1.1  1999/07/06 21:54:24  face
* introduced as new objects^
*

*/
#ifndef _nt3d_spot_lamp_h_
#define _nt3d_spot_lamp_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include "nt3d_lamp_obj.h"
#include "nt3d_hierarchic_object_obj.h"

/** @short NT3D_spot_lamp_o class

\bug unknown
\warning unknown
 */
class NT3D_spot_lamp_o : public NT3D_lamp_o {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NT3D_spot_lamp_o();
    NT3D_spot_lamp_o(const NT3D_spot_lamp_o & original);
    const NT3D_spot_lamp_o & operator= (const NT3D_spot_lamp_o & right);
    virtual ~NT3D_spot_lamp_o();
    /* ***  Methods              ***/
    /* ***  Data                 ***/

protected:
    /* ***  Methods              ***/
    void IlluminateLamp();
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    NTfloat exponent;
    NTfloat halfWidthAngle;
    GLfloat pDirectionV[3];
};
#endif /* _nt3d_spot_lamp_h_ */

/* File skeleton generated by GenNTObj version 0.5. */
