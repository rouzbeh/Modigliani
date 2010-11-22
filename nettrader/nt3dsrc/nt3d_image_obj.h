/**\file nt3d_image_obj.h - NT3D_image_o class header 
 * by Ahmed Aldo Faisal &copy; created 15.3.2000  
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
  

/* $Id: nt3d_image_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_image_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.1  2000/05/18 10:38:25  face
* an amazing error sits in nt3d_file_texture
* if the constructor constructs a texture(1,1,1)
* the system does not display the texture
* if (64,64,2) is specified it does.
* this is strange because the load routinge
* should adapt the object correctly...investigate here 2DO
*

*/
#ifndef _nt3d_image_obj_h_ 
#define _nt3d_image_obj_h_ 

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* Parent includes */
#include "nt_image_obj.h" 
/* NT includes */

/* other includes */
#include <GL/gl.h>
/** @short NT3D_image_o class 
\bug unknown
\warning unknown 
*/
class NT3D_image_o : public NT_image_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NT3D_image_o();
NT3D_image_o(const NT3D_image_o & original);
const NT3D_image_o & operator= (const NT3D_image_o & right);
virtual ~NT3D_image_o();
/* ***  Methods              ***/
GLfloat* RGBImagePtr() const;
GLfloat* RGBAImagePtr() const;
/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/  
private:
/* ***  Methods              ***/  
/* ***  Data                 ***/
};
#endif /* _nt3d_image_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
