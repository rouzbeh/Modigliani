/**\file nt3d_bitmap_obj.cpp - NT3D_bitmap_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 7.7.2000  
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

/* $Id: nt3d_bitmap_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: nt3d_bitmap_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/11/04 10:32:16  face
 * *** empty log message ***
 *

 */
#include "nt3d_bitmap_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_bitmap_o */
NT3D_bitmap_o::NT3D_bitmap_o(NTsize newWidth, NTsize newHeight,
		NTimageDataFormat newFormat) {
	width = newWidth;
	height = newHeight;
	format = newFormat;

	pIm = new GLshort[width * height * (int) format * sizeof(GLshort)];
}

/* ***      COPY AND ASSIGNMENT	***/
NT3D_bitmap_o::NT3D_bitmap_o(const NT3D_bitmap_o & original) :
		NT3D_o(), width(original.width), height(original.height), format(
				original.format) {
	pIm = new GLshort[width * height * (int) format * sizeof(GLshort)];
}

const NT3D_bitmap_o&
NT3D_bitmap_o::operator=(const NT3D_bitmap_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_bitmap_o::~NT3D_bitmap_o() {

}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 Ideas for possible extension of single channel code:
 - give possibility to get each of the three channels;
 - give possibility to get a grey (RGB) image;
 */
void NT3D_bitmap_o::Draw() {
	GLenum tmpFormat;

	switch (format) {
	case NT_RGB:
		tmpFormat = GL_RGB;
		break;
	case NT_RGBA:
		tmpFormat = GL_RGBA;
		break;
	default:
		NT_CERR(3, "NT3D_bitmap_o::Draw() - Error : Unsupported format yet.");
		break;
	}
	glRasterPos2f(0, 0);
	glDrawPixels(width, height, tmpFormat, GL_SHORT, pIm);
}

/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 Cut and save image from current view port

 _  (x2,y2)
 |


 |_ (x1,y1)

 Ideas for possible extension of single channel code:
 - give possibility to get each of the three channels;
 - give possibility to get a grey (RGB) image;
 */

void NT3D_bitmap_o::GetBitmap(NTsize x1, NTsize y1, NTsize x2, NTsize y2,
		NTimageDataFormat newFormat) {
	NTsize tWidth = x2 - x1;
	NTsize tHeight = y2 - y1;
	NT_ASSERT_PRECOND( (tWidth > 0) || (tHeight > 0));

	if ((tWidth != width) || (tHeight != height) || (format != newFormat)) {
		width = tWidth;
		height = tHeight;
		format = newFormat;
		delete[] pIm;
		pIm = new GLshort[width * height * (int) format * sizeof(GL_SHORT)];
	}

	GLenum tmpFormat;
	switch (format) {
	case NT_RGB:
		tmpFormat = GL_RGB;
		break;
	case NT_RGBA:
		tmpFormat = GL_RGBA;
		break;
	default:
		NT_CERR(3, "NT3D_bitmap_o::Draw() - Error : Unsupported format yet.");
		break;
	}
	glReadPixels(x1, y1, width, height, tmpFormat, GL_SHORT, pIm);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */