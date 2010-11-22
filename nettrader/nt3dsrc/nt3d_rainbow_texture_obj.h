/**\file nt3d_rainbow_texture_obj.h - NT3D_rainbow_texture_o class header 
 * by Ahmed Aldo Faisal &copy; created 27.4.2000  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.4
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
  

/* $Id: nt3d_rainbow_texture_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_rainbow_texture_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.7  2000/05/13 09:42:55  face
* *** empty log message ***
*
* Revision 1.6  2000/05/11 12:42:08  face
* removed nt3d_rainbow_o by intergrating it into nt3d_light_o
*
* Revision 1.5  2000/05/11 11:32:44  face
* upgrade to NetTrader Version 0.5.
* Common state of all files checkin (Versioon 0.5)
* rearranged color and material classes...
* texture class added
*
* Revision 1.4  2000/04/27 23:52:27  face
* further corrections
*
* Revision 1.3  2000/04/27 18:20:48  face
* loads of changes:
* -static init. fiasco workaounrd
* -texture objects have new structure
* ...
*
* Revision 1.2  2000/04/27 09:57:08  face
* switched to float based colors
* texture is RGBA color value,
* corrected nasty bug in nt3d_color_o, where the color bearing vector
* was initialised as 3 and not as 4-vector (RGBA!)NT3D_texture_o::CreateTexture
* :wq!
*
* Revision 1.1  2000/04/26 22:57:45  face
* added new classes with rainbow_texture as demo class
*

*/
#ifndef _nt3d_rainbow_texture_h_ 
#define _nt3d_rainbow_texture_h_ 

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* Parent includes */
#include "nt3d_pixel_texture_obj.h" 
/* NT includes */
#include "nt3d_color_obj.h"
/* other includes */

/** @short NT3D_rainbow_texture_o class 
\bug unknown
\warning unknown 
*/
class NT3D_rainbow_texture_o : public NT3D_pixel_texture_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NT3D_rainbow_texture_o(NTsize newTextureWidth=64, NTsize newTextureHeight=64);
NT3D_rainbow_texture_o(const NT3D_rainbow_texture_o & original);
const NT3D_rainbow_texture_o & operator= (const NT3D_rainbow_texture_o & right);
~NT3D_rainbow_texture_o();
/* ***  Methods              ***/  
/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/ 
NTreturn AssignPixelTexture();
/** Make a colored point within the normalised texure area [0,1]x[0,1] */
NTreal* MakeRGBATexturePoint(NTreal* colorVec, NTreal coorX, NTreal coorY);
/* ***  Data                 ***/  
private:
/* ***  Methods              ***/  
/* ***  Data                 ***/ 
};
#endif /* _nt3d_rainbow_texture_h_ */ 

/* File skeleton generated by GenNTObj version 0.6. */
