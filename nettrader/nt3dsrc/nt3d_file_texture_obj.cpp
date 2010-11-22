/**\file nt3d_file_texture_obj.cpp - NT3D_file_texture_o class implementation 
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
  

/* $Id: nt3d_file_texture_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_file_texture_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.1  2000/06/13 20:10:25  face
* *** empty log message ***
*

*/
#include "nt3d_file_texture_obj.h" 
#include "nt3d_image_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_file_texture_o */
NT3D_file_texture_o::NT3D_file_texture_o(string newFilename)
:
NT3D_texture_o(32, 32, 2)
{
	filename = newFilename;
}



/* ***      COPY AND ASSIGNMENT	***/ 
NT3D_file_texture_o::NT3D_file_texture_o(const NT3D_file_texture_o & original)
{
 // add assignment code here
}

const NT3D_file_texture_o&  
NT3D_file_texture_o::operator= (const NT3D_file_texture_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_file_texture_o::~NT3D_file_texture_o()
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
NT3D_file_texture_o::AssignTexture() 
{
	
	NT3D_image_o image;
	NTreturn tmp = image.Load(filename);

	/* first check that the texture data is ok */
	if (tmp != NT_SUCCESS) return tmp;
	
	cerr << textureWidth  << textureHeight<<endl;
	if 
		(!(
		(textureWidth == 1   || textureWidth == 2  || textureWidth == 4 || 
        textureWidth == 8   || textureWidth == 16 || 
        textureWidth == 32  || textureWidth == 64 ||
        textureWidth == 128 || textureWidth == 256 || textureWidth == 512) &&
        (textureWidth == 1   || textureHeight == 2  || textureHeight == 4 || 
        textureHeight == 8   || textureHeight == 16 || 
        textureHeight == 32  || textureHeight == 64 ||
        textureHeight == 128 || textureHeight == 256 || textureHeight == 512)
        ))
        {
	     cerr <<"NT3D_file_texture_o::NT3D_file_texture_o("<<filename
	     <<") - Error : texture width and/or height not a power of 2 or greater 2^8. Undefined future behaviour." <<endl;
	     return NT_BAD_DATA;
	    }

	/* then update the texture object */
	if ((1==textureWidth) || (1==textureHeight)) dim = GL_TEXTURE_1D;
	if ((1 > textureWidth) && (1 > textureHeight)) dim = GL_TEXTURE_2D;	

	textureWidth = image._width();
	textureHeight = image._height();
	delete[] textureImagePtr;
	textureImagePtr = new GLfloat[4*textureWidth*textureHeight];

	GLfloat* tmpPtr = image.RGBAImagePtr();
	for (NTsize ll = 0; ll < textureWidth*textureHeight*4; ll++){
		textureImagePtr[ll] = tmpPtr[ll];
		//cerr << textureImagePtr[ll] << "\t" << flush;
	}
	
	

	return NT_SUCCESS;
}
	

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
