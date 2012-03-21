/**\file nt3d_pixel_texture_obj.cpp - NT3D_pixel_texture_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 27.4.2000  
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
  

/* $Id: nt3d_pixel_texture_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_pixel_texture_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.6  2000/05/13 09:42:55  face
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
* Revision 1.3  2000/04/28 16:02:40  face
* found nasty hidden bug, by initialisation of an hideen ellipsoid making the
* entire program crash
* on destructor call... reason unknown
*
* Revision 1.2  2000/04/27 23:52:27  face
* further corrections
*
* Revision 1.1  2000/04/27 18:20:48  face
* loads of changes:
* -static init. fiasco workaounrd
* -texture objects have new structure
* ...
*

*/

#include "nt3d_pixel_texture_obj.h" 

#include <cmath>
#include <math.h>
#include "nt_aux_math_func.h"


/* ***      CONSTRUCTORS	***/
NT3D_pixel_texture_o::NT3D_pixel_texture_o(NTsize newTextureWidth, 
							   		NTsize newTextureHeight)
:
NT3D_texture_o(newTextureWidth, newTextureHeight)
{
	dim = GL_TEXTURE_2D;
}
							   


/* ***      COPY AND ASSIGNMENT	***/ 
NT3D_pixel_texture_o::NT3D_pixel_texture_o(const NT3D_pixel_texture_o & original):
		NT3D_texture_o(original.textureWidth, original.textureHeight, original.dim)
{
}

const NT3D_pixel_texture_o&  
NT3D_pixel_texture_o::operator= (const NT3D_pixel_texture_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_pixel_texture_o::~NT3D_pixel_texture_o()
{
	cerr << "NT3D_pixel_texture_o::~NT3D_pixel_texture_o()" << endl;
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.6. */
