/** \file nt3d_color_obj.cpp - NT template2 class implementation
 * by Ahmed Aldo Faisal &copy; created 15.10.1998  
 */
/* NetTrader - finance management, analysis and simulation system 
 * Version:   0.2						 
 * Copyright (C) 1998 Ahmed Aldo Faisal 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNES7S FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Id: nt3d_color_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_color_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2001/10/03 14:37:09  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.11  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.10  2000/05/13 09:42:55  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.8  2000/04/28 16:02:40  face
 * found nasty hidden bug, by initialisation of an hideen ellipsoid making the
 * entire program crash
 * on destructor call... reason unknown
 *
 * Revision 1.7  2000/04/27 23:52:27  face
 * further corrections
 *
 * Revision 1.6  2000/04/27 18:20:48  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.5  2000/04/27 09:57:08  face
 * switched to float based colors
 * texture is RGBA color value,
 * corrected nasty bug in nt3d_color_o, where the color bearing std::vector
 * was initialised as 3 and not as 4-std::vector (RGBA!)
 * :wq!
 *
 * Revision 1.4  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.3  1999/07/17 18:21:51  face
 * imodified
 7 *
 * Revision 1.2  1999/07/05 00:50:40  face
 * updated light color model
 *
 * Revision 1.1  1999/07/04 03:17:31  face
 * minor mods
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.3  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/11 02:26:40  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/07 23:53:04  face
 * Initial revision
 *
 * Revision 1.2  1999/01/07 12:56:15  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/27 11:53:40  face
 * Initial revision
 *
 *
 */

#include "nt3d_color_obj.h"

#include "nt_error_obj.h"
#include <GL/gl.h>

/** The following pre-defined color objects are helpfull speeding and
 * coding more complex computations 
 * However, there might be a "static initilization order fiasco"
 * as classes using them might be created before these are created.
 const NT3D_color_o NT3D_alpha(0.0,0.0,0.0,1.0);
 const NT3D_color_o NT3D_white(1.0,1.0,1.0,0.0);
 const NT3D_color_o NT3D_red(1.0,0.0,0.0,0.0);
 const NT3D_color_o NT3D_green(0.0,1.0,0.0,0.0);
 const NT3D_color_o NT3D_blue(0.0,0.0,1.0,0.0);
 const NT3D_color_o NT3D_black(0.0,0.0,0.0,0.0);
 *  to prevent this, the "Construct On First Use Idiom" is used
 *  instead of the objects, a function returning the identical
 *  object is used, but which prevents the fiasco as the - now -
 *  local static is constructed on the first sweep of the control flow
 */

NT3D_color_o& NT3D_alpha() {
	// cerr << "NT3D_color_o& NT3D_alpha()";
	static NT3D_color_o* vec = new NT3D_color_o(0.0, 0.0, 0.0, 1.0);
	return *vec;
}

NT3D_color_o& NT3D_white() {
	//cerr << "NT3D_color_o& NT3D_white()";
	static NT3D_color_o* vec = new NT3D_color_o(1.0, 1.0, 1.0, 0.0);
	return *vec;
}

NT3D_color_o& NT3D_red() {
	//cerr << "NT3D_color_o& NT3D_red()";
	static NT3D_color_o* vec = new NT3D_color_o(1.0, 0.0, 0.0, 0.0);
	return *vec;
}

NT3D_color_o& NT3D_green() {
	//cerr << "NT3D_color_o& NT3D_green()";
	static NT3D_color_o* vec = new NT3D_color_o(0.0, 1.0, 0.0, 0.0);
	return *vec;
}

NT3D_color_o& NT3D_blue() {
	//cerr << "NT3D_color_o& NT3D_blue()";
	static NT3D_color_o* vec = new NT3D_color_o(0.0, 0.0, 1.0, 0.0);
	return *vec;
}
NT3D_color_o& NT3D_yellow() {
	//cerr << "NT3D_color_o& NT3D_yellow()";
	static NT3D_color_o* vec = new NT3D_color_o(1.0, 1.0, 0.0, 0.0);
	return *vec;
}

NT3D_color_o& NT3D_black() {
	//cerr << "NT3D_color_o& NT3D_black()";
	static NT3D_color_o* vec = new NT3D_color_o(0.0, 0.0, 0.0, 0.0);
	return *vec;
}

/* 	CONSTRUCTORS 		*/
NT3D_color_o::NT3D_color_o() :
		colorRGBA(4) {
	colorPtr = new GLfloat[4];
	//cerr << "NT3D_color_o::NT3D_color_o()" << std::endl;
	SetColor(1, 0, 0, 1);
	M_ASSERT_DIM(4, colorRGBA.size());
}

NT3D_color_o::NT3D_color_o(GLfloat newR, GLfloat newG, GLfloat newB,
		GLfloat newAlpha) :
		colorRGBA(4) {
	colorPtr = new GLfloat[4];

	//cerr << "NT3D_color_o::NT3D_color_o(r,g,b,alpha)" << std::endl;
	SetColor(newR, newG, newB, newAlpha);
	M_ASSERT_DIM(4, colorRGBA.size());
}

NT3D_color_o::NT3D_color_o(const valarray<GLfloat> & newColorRGBA) :
		colorRGBA(4) {
	M_ASSERT_DIM(4, newColorRGBA.size());
	colorRGBA = newColorRGBA;
	M_ASSERT_DIM(4, colorRGBA.size());
}

/*
 NT3D_color_o::NT3D_color_o( const NT_std::vector_o & newColorRGBA)
 :
 colorRGBA(4)
 {
 colorPtr = new GLfloat[4];
 M_ASSERT_DIM( newColorRGBA.dim(), colorRGBA.dim() );
 //cerr << "NT3D_color_o::NT3D_color_o( rgbaVec)" << std::endl;
 colorRGBA = newColorRGBA;
 Update();
 M_ASSERT_DIM(4, colorRGBA.dim());
 }
 */

/* 	COPY AND ASSIGNMENT 	*/
NT3D_color_o::NT3D_color_o(const NT3D_color_o & original) :
		NT_o(), colorRGBA(4) {
	colorPtr = new GLfloat[4];
	M_ASSERT_DIM( colorRGBA.size(), original.colorRGBA.size());
	colorRGBA = original.colorRGBA;
	Update();
	M_ASSERT_DIM(4, colorRGBA.size());
}

const NT3D_color_o&
NT3D_color_o::operator=(const NT3D_color_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	colorPtr = new GLfloat[4];
	M_ASSERT_DIM( colorRGBA.size(), right.colorRGBA.size());
	colorRGBA = right.colorRGBA;
	M_ASSERT_DIM(4, colorRGBA.size());
	return *this;
}

/** 	DESTRUCTOR 	*/
NT3D_color_o::~NT3D_color_o() {
	cerr << "NT3D_color_o::~NT3D_color_o() - Warning: colorPtr not deleted."
			<< std::endl;
	M_ASSERT_DIM(4, colorRGBA.size());
	// 2DO Clarify why the color pointer cannot be delted : delete colorPtr;
}

/*   	PUBLIC                	*/

/** @short  SetColor
 @param 	none
 @return 	none
 \warning 	unknown
 \bug 	unknown
 */
void NT3D_color_o::SetColor(GLfloat newR, GLfloat newG, GLfloat newB,
		GLfloat newAlpha) {
	colorRGBA[0] = newR;
	colorRGBA[1] = newG;
	colorRGBA[2] = newB;
	colorRGBA[3] = newAlpha;
	Update();
}

/** @short  SetColor
 @param 	none
 @return 	none
 \warning 	unknown
 \bug 	unknown
 */
void NT3D_color_o::SetColor(const GLfloat newColor[]) {
	M_ASSERT_DIM(4, colorRGBA.size());
	mbase::Msize ll;
	for (ll = 0; ll < 5; ll++)
		colorRGBA[ll] = newColor[ll];
	Update();
}

/** @short  SetColor
 @param 	none
 @return 	none
 \warning 	unknown
 \bug 	unknown
 */
void NT3D_color_o::SetColor(const NT3D_color_o & newColor) {
	colorRGBA = newColor.colorRGBA;
	Update();
	M_ASSERT_DIM(4, colorRGBA.size());
}

// Arithmetics
NT3D_color_o NT3D_color_o::operator+(const NT3D_color_o &right) const {
	return NT3D_color_o(colorRGBA + right.colorRGBA);
}

NT3D_color_o NT3D_color_o::operator-(const NT3D_color_o &right) const {
	return NT3D_color_o(colorRGBA - right.colorRGBA);
}

// scalar multiplication from the RIGHT side
NT3D_color_o NT3D_color_o::operator*(mbase::Real alpha) const {
	NT3D_color_o tmp(colorRGBA);
	tmp.colorRGBA *= alpha;
	M_ASSERT_DIM(4, colorRGBA.size());
	return tmp;
}

/*   	PROTECTED     	      	*/
/** @short 	Update
 @param 	none
 @return 	none
 \warning 	unknown
 \bug 	unknown
 */
void NT3D_color_o::Update() {
	mbase::Msize ll;
	for (ll = 0; ll < 5; ll++)
		colorPtr[ll] = colorRGBA[ll];
}

/* 	PRIVATE               	*/

