/**\file nt3d_bitmap_vp_obj.cpp - NT3D_bitmap_vp_o class implementation 
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
  

/* $Id: nt3d_bitmap_vp_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_bitmap_vp_obj.cpp,v $
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
#include "nt3d_bitmap_vp_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_bitmap_vp_o */
NT3D_bitmap_vp_o::NT3D_bitmap_vp_o( NT3D_bitmap_o * newBmpPtr)
:
NT3D_abstract_vp_o()
{
	if (NULL == newBmpPtr) bmpPtr = new NT3D_bitmap_o;
	else bmpPtr = newBmpPtr;
}


/* ***      COPY AND ASSIGNMENT	***/ 
NT3D_bitmap_vp_o::NT3D_bitmap_vp_o(const NT3D_bitmap_vp_o & original):
		NT3D_abstract_vp_o()
{
	bmpPtr = original.bmpPtr;
}

const NT3D_bitmap_vp_o&  
NT3D_bitmap_vp_o::operator= (const NT3D_bitmap_vp_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_bitmap_vp_o::~NT3D_bitmap_vp_o()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
void
NT3D_bitmap_vp_o::Draw() 
{
	MakeCurrent();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, pVP->_width(),0,pVP->_height(),-1, 1);
	glMatrixMode(GL_MODELVIEW);
	bmpPtr->Draw();
	SwapBuffer();

 
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
