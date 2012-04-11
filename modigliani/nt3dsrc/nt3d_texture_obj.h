/**\file nt3d_texture_obj.h - NT3D_texture_o class header
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
  

/* $Id: nt3d_texture_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_texture_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.10  2000/06/26 19:14:06  face
* finally found the error that caused solid object to disappear:
* the scaling vector was not constructed in the 2nd constructor
* of the solid bject thus resulting in a zero object
* new demo file: showing some random graphics
*
* Revision 1.9  2000/06/13 20:10:26  face
* *** empty log message ***
*
* Revision 1.8  2000/05/18 10:38:25  face
* an amazing error sits in nt3d_file_texture
* if the constructor constructs a texture(1,1,1)
* the system does not display the texture
* if (64,64,2) is specified it does.
* this is strange because the load routinge
* should adapt the object correctly...investigate here 2DO
*
* Revision 1.7  2000/05/13 09:42:55  face
* *** empty log message ***
*
* Revision 1.6  2000/05/11 12:42:08  face
* removed nt3d_rainbow_o by intergrating it into nt3d_light_o
*
* Revision 1.5  2000/04/28 16:02:40  face
* found nasty hidden bug, by initialisation of an hideen ellipsoid making the
* entire program crash
* on destructor call... reason unknown
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
* was initialised as 3 and not as 4-vector (RGBA!)
* :wq!
*
* Revision 1.1  2000/04/26 22:57:45  face
* added new classes with rainbow_texture as demo class
*

*/
#ifndef _nt3d_texture_h_ 
#define _nt3d_texture_h_ 

/* NT core includes */
#include "ntsrc/nt_main.h" 
#include "ntsrc/nt_types.h" 
#include "ntsrc/nt_obj.h"  
/* NT includes */
#include "nt3d_obj.h"
/* other includes */
#include <GL/glu.h>



/** @short NT3D_texture_o class 
This base class provides data independent facilities
to manipulate and administrate textures.
The texture is bound to the object through the CreateTexture()
call in the NT3D_texture_o constructor. 

General drawing usage in a derived NT3D_object_o class is:
Paint() -> DrawXYZ() -> Painted()

\bug unknown
\warning dim is 2 by default in this base class
*/ 



class NT3D_texture_o : public NT_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NT3D_texture_o(NTsize newTextureWidth=64, 
			   NTsize newTextureHeight=64, 
			   NTsize newDim=2,
			   bool newMakeTextureCoord=false);		   
NT3D_texture_o(const NT3D_texture_o & original);
const NT3D_texture_o & operator= (const NT3D_texture_o & right);
~NT3D_texture_o();
/* ***  Methods              ***/  
/**  After Paint() texture coordinates should be specified 
	 in the control flow. */
void Paint(); 
/** to be called after to be textured objects were drawn */
void Painted() { glDisable(dim); };
/** Switches texture coord generation on and off in the pipeline */
void TextureCoord ( bool bSwitch) { makeTextureCoord = bSwitch;}
/* ***  Data                 ***/  

protected:
/* ***  Methods              ***/  
virtual NTreturn AssignTexture() { NT_CERR(1,"NT3D_texture_o::AssignTexture - Error : Not overridden in derived class."); return NT_FAIL;};
/* ***  Data                 ***/  
GLfloat *textureImagePtr;
GLuint textureId;
NTsize textureWidth;
NTsize textureHeight;
GLenum dim;

private:
/* ***  Methods              ***/
GLuint CreateTexture();
/* ***  Data                 ***/ 
bool textureCreated;
bool makeTextureCoord;
//static NT_id_management_o idManager;
};
#endif /* _nt3d_texture_h_ */ 

/* File skeleton generated by GenNTObj version 0.6. */
