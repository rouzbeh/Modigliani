/**\file nt3d_worldmap_texture_obj.cpp - NT3D_worldmap_texture_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 12.5.2000  
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
  

/* $Id: nt3d_worldmap_texture_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_worldmap_texture_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.2  2002/02/26 00:00:59  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.2  2000/05/18 10:38:25  face
* an amazing error sits in nt3d_file_texture
* if the constructor constructs a texture(1,1,1)
* the system does not display the texture
* if (64,64,2) is specified it does.
* this is strange because the load routinge
* should adapt the object correctly...investigate here 2DO
*
* Revision 1.1  2000/05/13 09:43:29  face
* *** empty log message ***
*

*/
#include "nt3d_worldmap_texture_obj.h" 

#include "nt_worldmap_image_data.h"
#include <cmath>


/* ***      CONSTRUCTORS	***/
/** Create a NT3D_worldmap_texture_o */
NT3D_worldmap_texture_o::NT3D_worldmap_texture_o()
:
NT3D_texture_o(128,64,2)
{
}
/* ***      COPY AND ASSIGNMENT	***/ 
NT3D_worldmap_texture_o::NT3D_worldmap_texture_o(const NT3D_worldmap_texture_o & original):
		NT3D_texture_o(original.textureWidth, original.textureHeight, original.dim)
{
}

const NT3D_worldmap_texture_o&  
NT3D_worldmap_texture_o::operator= (const NT3D_worldmap_texture_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_worldmap_texture_o::~NT3D_worldmap_texture_o()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
NTreturn
NT3D_worldmap_texture_o::AssignTexture() 
{
	unsigned int * tmpPtr = NT_worldmap_RGB_uByte_128_64_image;
	NTsize i;
   	NTsize val;
	 for (i = 0; i < 3*textureWidth*textureHeight; i += 3) {
			                           
			val = i + (int) floor(i/3.0);
			
			textureImagePtr[val ]  = tmpPtr[i]/255.0;
            textureImagePtr[val+1] = tmpPtr[i+1]/255.0;
            textureImagePtr[val+2] = tmpPtr[i+2]/255.0;
            textureImagePtr[val+3] = 0.5;
    }
    
    
    return NT_SUCCESS;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */




/* File skeleton generated by GenNTObj version 0.7. */
