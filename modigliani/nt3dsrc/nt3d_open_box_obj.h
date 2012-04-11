/**\file nt3d_open_box_obj.h - NT3D_open_box_o class header 
 * by Ahmed Aldo Faisal &copy; created 18.5.2000  
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
  

/* $Id: nt3d_open_box_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_open_box_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.2  2000/07/06 08:06:10  face
* *** empty log message ***
*
* Revision 1.1  2000/06/13 20:10:25  face
* *** empty log message ***
*

*/
#ifndef _nt3d_open_box_obj_h_ 
#define _nt3d_open_box_obj_h_ 

/* NT core includes */
#include "ntsrc/nt_main.h" 
#include "ntsrc/nt_types.h" 
#include "ntsrc/nt_obj.h"  
/* Parent includes */
#include "nt3d_surfaced_object_obj.h" 
/* NT includes */
#include "ntsrc/nt_vector3_obj.h"
#include "nt3d_material_obj.h"
#include "nt3d_texture_obj.h"
/* other includes */

/** @short NT3D_open_box_o class 
 ** A box without bottom and top, with the walls beeing parallel to the width
 ** , i.e. the Z axis 
\bug unknown
\warning unknown 
*/
class NT3D_open_box_o : public NT3D_surfaced_object_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NT3D_open_box_o(NTreal width = 1.0, NTreal depth = 1.0, NTreal height = 1.0);
NT3D_open_box_o(const NT3D_open_box_o & original);
const NT3D_open_box_o & operator= (const NT3D_open_box_o & right);
virtual ~NT3D_open_box_o();
/* ***  Methods              ***/   
/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/
void DrawSurfacedObj();
virtual void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl , NTint __attribute__((unused)) value){cerr << "Not implemented" << endl;};
virtual void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl , NTreal __attribute__((unused)) value){cerr << "Not implemented" << endl;};
virtual void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl , NT_vector_o __attribute__((unused)) value){cerr << "Not implemented" << endl;};
/* ***  Data                 ***/  
NT_vector3_o oScaling;
private:
/* ***  Methods              ***/  
/* ***  Data                 ***/ 
NTreal width;
NTreal depth;
NTreal height;

};
#endif /* _nt3d_open_box_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
