/**\file nt3d_image_obj.cpp - NT3D_image_o class implementation 
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
  

/* $Id: nt3d_image_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_image_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.2  2002/03/04 18:08:33  face
* added checking for NaN and Inf values in find_spikes_in_binary_file-
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
#include "nt3d_image_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_image_o */
NT3D_image_o::NT3D_image_o()
:
NT_image_o()
{

}
/* ***      COPY AND ASSIGNMENT	***/ 
NT3D_image_o::NT3D_image_o(const NT3D_image_o & original)
{
	 // add assignment code here
	
}

const NT3D_image_o&  
NT3D_image_o::operator= (const NT3D_image_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_image_o::~NT3D_image_o()
{
 
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return    An initialised array of (width X height) RGB triples,
    		   the array is to be deleted[] by the user.
   \warning    unknown
   \bug        unknown
 */
GLfloat*
NT3D_image_o::RGBImagePtr() const
{
	GLfloat* imagePtr = new GLfloat[width*height*3];
	cerr <<"NT3D_image_o::RGBImagePt - Talk : Large potential for memory loss here, as long as memory pointer returned."<<endl;
	NTsize ll;
	switch (dataFormat) {
		case NT_SINGLE_CHANNEL:
			for (ll=0; ll < width*height; ll++) {
				imagePtr[4*ll]   = _imageVecR(ll);
				imagePtr[4*ll+1] = _imageVecR(ll);
				imagePtr[4*ll+2] = _imageVecR(ll);
				
			}
			break;
		case NT_RGB: 
			for (ll=0; ll < 3*width*height; ll++) {
				imagePtr[ll]   = _imageVecR(ll);
				}
		 	break;
		case NT_RGBA: 
				for (ll=0; ll < width*height; ll++) {
				imagePtr[3*ll]   = _imageVecR(4*ll);
				imagePtr[3*ll+1] = _imageVecR(4*ll+1);
				imagePtr[3*ll+2] = _imageVecR(4*ll+2);
				}
			break;
		default : 
			cerr << "NT3D_image_o::getRGBAImagePtr - Error : Unimplmented/Unsupported data format, dataFormat="<<dataFormat<<"." <<endl;
			return NULL;
	}
	return imagePtr;
}

/** @short       
    @param      none
    @return    An initialised array of (width X height) RGBA quadruples,
    		   the array is to be deleted[] by the user.
   \warning    unknown
   \bug        unknown
 */
GLfloat*
NT3D_image_o::RGBAImagePtr() const
{
	cerr <<"NT3D_image_o::RGBAImagePtr()..."<<endl;
	GLfloat* imagePtr = new GLfloat[width*height*4];
	cerr <<"NT3D_image_o::RGBImagePt - Talk : Large potential for memory loss here, as long as new pointer is returned."<<endl;
	NTsize ll;
	switch (dataFormat) {
		case NT_SINGLE_CHANNEL:
			for (ll=0; ll < width*height; ll++) {
				imagePtr[4*ll]   = _imageVecR(ll);
				imagePtr[4*ll+1] = _imageVecR(ll);
				imagePtr[4*ll+2] = _imageVecR(ll);
				imagePtr[4*ll+3] = 0.5;
			}
			break;
		case NT_RGB: 
			for (ll=0; ll < width*height; ll++) {
				imagePtr[4*ll]   = _imageVecR(3*ll);
				imagePtr[4*ll+1] = _imageVecR(3*ll+1);
				imagePtr[4*ll+2] = _imageVecR(3*ll+2);
				imagePtr[4*ll+3] = 0.5;
			}
		 	break;
		case NT_RGBA: 
				for (ll=0; ll < 4*width*height; ll++)
							imagePtr[ll]   = _imageVecR(ll);
			break;
		default : 
			cerr << "NT3D_image_o::getRGBAImagePtr - Error : Unimplmented/Unsupported data format, dataFormat="<<dataFormat<<"." <<endl;
			return NULL;
	}

	cerr <<"NT3D_image_o::RGBAImagePtr() completed."<<endl;
	return imagePtr;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
