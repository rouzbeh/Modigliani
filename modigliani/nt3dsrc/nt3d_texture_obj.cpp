/**\file nt3d_texture_obj.cpp - NT3D_texture_o class implementation
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
  

/* $Id: nt3d_texture_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_texture_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.2  2003/02/17 10:20:20  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.11  2000/06/26 19:14:06  face
* finally found the error that caused solid object to disappear:
* the scaling std::vector was not constructed in the 2nd constructor
* of the solid bject thus resulting in a zero object
* new demo file: showing some random graphics
*
* Revision 1.10  2000/06/13 20:10:26  face
* *** empty log message ***
*
* Revision 1.9  2000/05/18 10:38:25  face
* an amazing error sits in nt3d_file_texture
* if the constructor constructs a texture(1,1,1)
* the system does not display the texture
* if (64,64,2) is specified it does.
* this is strange because the load routinge
* should adapt the object correctly...investigate here 2DO
*
* Revision 1.8  2000/05/13 09:42:55  face
* *** empty log message ***
*
* Revision 1.7  2000/05/11 12:42:08  face
* removed nt3d_rainbow_o by intergrating it into nt3d_light_o
*
* Revision 1.6  2000/05/11 11:32:44  face
* upgrade to NetTrader Version 0.5.
* Common state of all files checkin (Versioon 0.5)
* rearranged color and material classes...
* texture class added
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
* corrected nasty bug in nt3d_color_o, where the color bearing std::vector
* was initialised as 3 and not as 4-std::vector (RGBA!)
* :wq!
*
* Revision 1.1  2000/04/26 22:57:45  face
* added new classes with rainbow_texture as demo class
*

*/
#include "nt3d_texture_obj.h" 



//NT_id_management_o NT3D_texture_o::idManager= NT_id_management_o(2,NT3D_MAX_TEXTURES - 1);



/* ***      CONSTRUCTORS	***/
/** Create a NT3D_texture_o */
NT3D_texture_o::NT3D_texture_o(mbase::Msize newTextureWidth, 
							   mbase::Msize newTextureHeight,
							   mbase::Msize newDim,
							   bool newMakeTextureCoord)
{
//cerr<< "NT3D_texture_o::NT3D_texture_o() - Talk : uniqId="<<_uniqId() << std::endl;

textureCreated = false;
textureId = 0;
textureWidth = newTextureWidth;
textureHeight = newTextureHeight;
makeTextureCoord = newMakeTextureCoord;

if (2 == newDim ) {
	dim = GL_TEXTURE_2D;
	if ((1==textureWidth) || (1==textureHeight)) { 
		dim = GL_TEXTURE_1D;
		NT_CERR(3,"NT3D_texture_o::NT3D_texture_o - Warning : dim declared 2, but height and/or width == 1. dim set now to 1.");
	}
} else if (1 == newDim ) {
	dim = GL_TEXTURE_1D;
	if ((1 > textureWidth) && (1 > textureHeight)) {
		dim = GL_TEXTURE_2D;
		NT_CERR(3,"T3D_texture_o::NT3D_texture_o - Warning : dim declared 1, but both height and width greater 1. dim set now to 2.");
	}
} else cerr << "NT3D_texture_o::NT3D_texture_o - Error : invalid texture dim="<<newDim<<"specified." << std::endl;

textureImagePtr = new GLfloat[4 * textureWidth * textureHeight];
cerr<< "NT3D_texture_o::NT3D_texture_o() - Talk : uniqId="<<_uniqId() << std::endl;
}



/* ***      COPY AND ASSIGNMENT	***/ 
/** The copy constructor copies all aspects of the texture, but binds it to a new, different textureId */
NT3D_texture_o::NT3D_texture_o(const NT3D_texture_o & original):
		NT_o()
{
 NT_CERR(7,"NT3D_texture_o::NT3D_texture_o - Talk : copy and assignment called.");


 textureWidth = original.textureWidth;
 textureHeight = original.textureHeight;
 dim = original.dim;
 textureImagePtr = new GLfloat[4*textureWidth*textureHeight];
 for (mbase::Msize ll = 0 ; ll < 4*textureWidth*textureHeight; ll++)
	textureImagePtr[ll] = original.textureImagePtr[ll];

 CreateTexture();
}



const NT3D_texture_o&  
NT3D_texture_o::operator= (const NT3D_texture_o & right)
{
 NT_CERR(7,"NT3D_texture_o::operator= - Talk: operator= called.");
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
	textureWidth = right.textureWidth;
	textureHeight = right.textureHeight;
	dim = right.dim;

	if (true == textureCreated){
		GLuint tmp = textureId;
		glDeleteTextures(1, &tmp);
	}
	
	delete[] textureImagePtr;
	textureImagePtr = new GLfloat[4*textureWidth*textureHeight];

 for (mbase::Msize ll = 0 ; ll < 4*textureWidth*textureHeight; ll++)
	textureImagePtr[ll] = right.textureImagePtr[ll];

 CreateTexture();

 return *this;
}




/* ***      DESTRUCTOR		***/
NT3D_texture_o::~NT3D_texture_o()
{
	NT_CERR(9,"NT3D_texture_o::~NT3D_texture_o()");
	GLuint tmp = textureId;
	glDeleteTextures(1, &tmp);
	delete[] textureImagePtr;
	cerr << "...completed" << std::endl;
}

/* ***  PUBLIC                                    ***   */  
/** @short      Paint sets up texture-ing, such that hereafter
	 		    texture coordinates should be provided to the control flow
    @param      none
    @return     none
   \warning    Call only after a rendering context was defined/opened
   			   otherwise the system will crash here with SIGSEV
   \bug        unknown
 */
 void NT3D_texture_o::Paint()
 {
 	//cerr << "NT3D_texture_o::Paint() - Talk : before calling drawing routines, runnId="<< _uniqId() << std::endl;
 	if (true == textureCreated) {
 		glEnable(dim); 
 		glBindTexture(dim, textureId);
 	}
 	else CreateTexture(); glBindTexture(dim, textureId);
 	//cerr << "NT3D_texture_o::Paint() - Talk : after drawing routines, runnId="<< _uniqId() << std::endl;
 }

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
/** @short        
    @param      none
    @return     OpenGL textureId or 0 if failed.
   \warning    Call only after a rendering context was defined/opened
   			   otherwise the system will crash here with SIGSEV
   \bug        unknown
 */
GLuint
NT3D_texture_o::CreateTexture()
{
	if (true == textureCreated) return textureId;
	//cerr << "NT3D_texture_o::CreateTexture() - Talk : before call to AssignTexture, runnId="<< _uniqId() << std::endl;
	mbase::Mreturn tmpReturn = AssignTexture();
	//cerr << "NT3D_texture_o::CreateTexture() - Talk : after call to AssignTexture" << std::endl;
	if (mbase::M_SUCCESS != tmpReturn){
		NT_CERR(1,"NT3D_texture_o::CreateTexture() - Error : AssignTexture failed.");
		textureCreated = false;
		return 0;
	}

/*  THIS MAKES REMOVES A LOT OF TEXTURED SURFACES...DO NOT USE
	DO NOT USE glEnable(GL_CULL_FACE);
	DO NOT USE glFrontFace(GL_CW);
	DO NOT USE glCullFace(GL_BACK);
  */
    glEnable(dim);
	glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);		


	// Work around code, for compilers that do not handle pointers to members well:
	GLuint tmp = 0;
	glGenTextures(1, &tmp);
	textureId = tmp;
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	
	/*
	glGenTextures(1, &textureId);
	glBindTexture(dim, textureId);
	*/
	



	GLfloat objPlaneVec[] = {1.0, 0.0, 0.0, 0.0};

	glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   	glTexParameteri(dim, GL_TEXTURE_WRAP_S, GL_REPEAT);
   	if (true == makeTextureCoord) {
		glEnable(GL_TEXTURE_GEN_S);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, objPlaneVec);
	} else {
		glDisable(GL_TEXTURE_GEN_S);
	}
   	if (GL_TEXTURE_2D == dim) {
		glTexParameteri(dim, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (true == makeTextureCoord) {
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   			glTexGenfv(GL_T, GL_OBJECT_PLANE, objPlaneVec);
     	} else {
     		glDisable(GL_TEXTURE_GEN_T);
     	}    		
   	}

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	// glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);


	/* assign the texture dim specific */
	if (GL_TEXTURE_1D == dim) 
				glTexImage1D(GL_TEXTURE_1D, 0 , GL_RGBA, (textureWidth > textureHeight) ? textureWidth : textureHeight, 0,
			     			 GL_RGBA, GL_FLOAT, textureImagePtr);
	else if (GL_TEXTURE_2D == dim) 
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0,
       		     			 GL_RGBA, GL_FLOAT, textureImagePtr);
	else {
		cerr << "NT3D_texture_o::CreateTexture() - Error : invalid texture dim="<<dim<<"specified." <<endl;
		textureCreated = false;
		return mbase::M_FAIL;
		}	
       		     			 

    textureCreated = true;
   	return textureId;
}


/* File skeleton generated by GenNTObj version 0.6. */
