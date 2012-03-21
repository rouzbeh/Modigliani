/**\file nt3d_noise_texture_obj.cpp - NT3D_noise_texture_o class implementation 
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
  

/* $Id: nt3d_noise_texture_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_noise_texture_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.4  2000/05/13 09:42:55  face
* *** empty log message ***
*
* Revision 1.3  2000/05/11 11:32:44  face
* upgrade to NetTrader Version 0.5.
* Common state of all files checkin (Versioon 0.5)
* rearranged color and material classes...
* texture class added
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
#include "nt3d_noise_texture_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_noise_texture_o */
NT3D_noise_texture_o::NT3D_noise_texture_o(NTsize newTextureWidth, 
							               NTsize newTextureHeight)
:
NT3D_pixel_texture_o(newTextureWidth, newTextureHeight)
{
}

/* ***      COPY AND ASSIGNMENT	***/ 
NT3D_noise_texture_o::NT3D_noise_texture_o(const NT3D_noise_texture_o & original):
		NT3D_pixel_texture_o(original.textureWidth, original.textureHeight)
{
}

const NT3D_noise_texture_o&  
NT3D_noise_texture_o::operator= (const NT3D_noise_texture_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_noise_texture_o::~NT3D_noise_texture_o()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
NTreal *
NT3D_noise_texture_o::MakeRGBATexturePoint(NTreal* colorVec, NTreal __attribute__((unused)) coorX, NTreal __attribute__((unused)) coorY)
{
		
	colorVec[0] = rRnd.RndVal();
	colorVec[1] = gRnd.RndVal();
 	colorVec[2] = bRnd.RndVal();
	colorVec[3] = 1.0;
	return colorVec;
}

/* ***  PROTECTED                         ***   */

NTreturn
NT3D_noise_texture_o::AssignPixelTexture()
{
	NTsize i, j;
    NTreal ti, tj;
	NTsize tmp;
	NTreal* tmpColor = new NTreal[4];

    for (i = 0; i < textureWidth; i++) {
        ti = 2.0*M_PI*i/textureWidth;
        for (j = 0; j < textureHeight; j++) {
            tj = 2.0*M_PI*j/textureHeight;
            tmpColor = MakeRGBATexturePoint(tmpColor, ti, tj);
			tmp = (textureHeight*i+j);
			textureImagePtr[4*tmp  ]   = tmpColor[0];
            textureImagePtr[(4*tmp)+1] = tmpColor[1];
            textureImagePtr[(4*tmp)+2] = tmpColor[2];
            textureImagePtr[(4*tmp)+3] = tmpColor[3];
         }
    }
    delete[] tmpColor;
    
    return NT_SUCCESS;
}


/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.6. */
