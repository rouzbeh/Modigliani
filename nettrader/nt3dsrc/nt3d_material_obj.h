/**\file nt3d_material_obj.h - NT3D_material_o class header
 * by Ahmed Aldo Faisal &copy; created 3.7.1999
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

/* $Id: nt3d_material_obj.h,v 1.1 2001/06/29 13:16:55 face Exp $
* $Log: nt3d_material_obj.h,v $
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.5  2000/05/11 11:32:44  face
* upgrade to NetTrader Version 0.5.
* Common state of all files checkin (Versioon 0.5)
* rearranged color and material classes...
* texture class added
*
* Revision 1.4  2000/04/27 18:20:48  face
* loads of changes:
* -static init. fiasco workaounrd
* -texture objects have new structure
* ...
*
* Revision 1.3  2000/04/26 17:10:34  face
* minor bug fixed
* and upgrades:
*
* Revision 1.2  1999/07/05 00:50:45  face
* updated light color model
*
* Revision 1.1  1999/07/04 03:17:33  face
* minor mods
*

*/
#ifndef _nt3d_material_h_
#define _nt3d_material_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include "nt3d_color_obj.h"

/** @short NT3D_material_o class

\bug unknown
\warning unknown
 */
class NT3D_material_o : public NT_o {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NT3D_material_o();
    NT3D_material_o(const NT3D_material_o & original);
    const NT3D_material_o & operator= (const NT3D_material_o & right);
    ~NT3D_material_o();
    /* ***  Methods              ***/
    void Paint();
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    void Init();
    void UpdateMaterial();
    /* ***  Data                 ***/
    NT3D_color_o colorAmbient;
    NT3D_color_o colorDiffuse;
    NT3D_color_o colorSpecular;
    NTfloat     colorShininess;
    NT3D_color_o colorEmission;
};
#endif /* _nt3d_material_h_ */

/* File skeleton generated by GenNTObj version 0.5. */
