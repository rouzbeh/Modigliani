/**\file nt3d_surfaced_object_obj.cpp - NT3D_surfaced_object_o class implementation 
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
  

/* $Id: nt3d_surfaced_object_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_surfaced_object_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.3  2000/07/06 08:06:10  face
* *** empty log message ***
*
* Revision 1.2  2000/06/26 19:14:06  face
* finally found the error that caused solid object to disappear:
* the scaling vector was not constructed in the 2nd constructor
* of the solid bject thus resulting in a zero object
* new demo file: showing some random graphics
*
* Revision 1.1  2000/06/13 20:10:26  face
* *** empty log message ***
*

*/
#include "nt3d_surfaced_object_obj.h" 

#include "nt3d_rainbow_texture_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_surfaced_object_o */
NT3D_surfaced_object_o::NT3D_surfaced_object_o()
{
	  textureSet = false;
	  bUseTexture = true;
	  ResetTexture();
	  NT_CERR(11,"NT3D_surfaced_object_o::NT3D_surfaced_object_o()  - Talk : constructor called.");
}

/** Create a NT3D_surfaced_object_o */
NT3D_surfaced_object_o::NT3D_surfaced_object_o(const NT_vector3_o & oPosition)
:
NT3D_hierarchic_object_o(oPosition)
{
  textureSet = false;
  bUseTexture = true;
  ResetTexture();
  NT_CERR(11,"NT3D_surfaced_object_o::NT3D_surfaced_object_o(oPosition)  - Talk : constructor called.");
}

/* ***      COPY AND ASSIGNMENT	***/ 
NT3D_surfaced_object_o::NT3D_surfaced_object_o(const NT3D_surfaced_object_o & original)
{
  if (true == original.textureSet) {
  	NT3D_texture_o* tmpTexturePtr = new NT3D_texture_o(*(original.oTexturePtr));
  	SetTexture( tmpTexturePtr);
  } else {
  	textureSet = false;
  }
  
  // cerr << "NT3D_surfaced_object_o::NT3D_surfaced_object_o(const ...) - Talk : oTexturePtr->RunningId="<< oTexturePtr->_uniqId() << "." << endl;
}

const NT3D_surfaced_object_o&  
NT3D_surfaced_object_o::operator= (const NT3D_surfaced_object_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment

 // Different copy behaviours depending whether the right side has a texture set 
 if (true == right.textureSet) {
 	NT3D_texture_o* tmpTexturePtr = new NT3D_texture_o(*(right.oTexturePtr));
 	SetTexture( tmpTexturePtr);
 
  } else {
  	textureSet = false;
   	delete oTexturePtr;
  }
  return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_surfaced_object_o::~NT3D_surfaced_object_o()
{
	if (true == textureSet) delete oTexturePtr;
}

/* ***  PUBLIC                                    ***   */  

NTreturn 
NT3D_surfaced_object_o::SetTexture( NT3D_texture_o* newTexturePointer )
{
  // 2DO fix LEAK but delete works badly here	if (true == textureSet) delete oTexturePtr;
	oTexturePtr = newTexturePointer;
	textureSet = true;
	return NT_SUCCESS;
}

/* ***  PROTECTED                         ***   */
NTreturn 
NT3D_surfaced_object_o::ResetTexture()
{
  // 2DO fix LEAK but delete works badly here	if (true == textureSet) delete oTexturePtr;

	oTexturePtr = new NT3D_rainbow_texture_o;
	textureSet = true;
	return NT_SUCCESS;
}

void 
NT3D_surfaced_object_o::ControlObj(NT3Dcontroller cntrl , NTint value)
{
	switch	(cntrl)	{
		case NT3D_Texture:  
							bUseTexture = (bool)value;
							cerr << "set bUseTexture." <<endl;
		 	 				break;
		default:
							break;
	}
	
	ControlSurfacedObj(cntrl, value);	
}

/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
