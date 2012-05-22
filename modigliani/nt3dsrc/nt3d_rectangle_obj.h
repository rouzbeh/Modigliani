/**\file nt3d_rectangle_obj.h - NT3D_rectangle_o class header 
 * by Ahmed Aldo Faisal &copy; created 19.5.2000  
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
  

/* $Id: nt3d_rectangle_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_rectangle_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.2  2000/06/26 19:14:06  face
* finally found the error that caused solid object to disappear:
* the scaling std::vector was not constructed in the 2nd constructor
* of the solid bject thus resulting in a zero object
* new demo file: showing some random graphics
*
* Revision 1.1  2000/06/13 20:10:26  face
* *** empty log message ***
*

*/
#ifndef _nt3d_rectangle_obj_h_ 
#define _nt3d_rectangle_obj_h_ 

/* NT core includes */
#include "mbase/nt_main.h" 
#include "mbase/nt_types.h" 
#include "mbase/nt_obj.h"  
/* Parent includes */
#include "nt3d_surfaced_object_obj.h" 
/* NT includes */

/* other includes */

/** @short NT3D_rectangle_o class 
 ** Draws an recangular area composed of steps^2 rectanlges which
 ** lies flat in the X-Y-plane (width-height) with the lower left corner
 ** in the origin
\bug unknown
\warning unknown 
*/
class NT3D_rectangle_o : public NT3D_surfaced_object_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NT3D_rectangle_o(mbase::Real width = 1.0, mbase::Real height = 1.0, mbase::Msize steps = 2);
NT3D_rectangle_o(const NT3D_rectangle_o & original);
const NT3D_rectangle_o & operator= (const NT3D_rectangle_o & right);
virtual ~NT3D_rectangle_o();
/* ***  Methods              ***/ 

/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/  
private:
/* ***  Methods              ***/
void DrawSurfacedObj(); 
void ConstructRectanlge();
/* ***  Data                 ***/ 
mbase::Real width;
mbase::Real height;
mbase::Msize steps;
};
#endif /* _nt3d_rectangle_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
