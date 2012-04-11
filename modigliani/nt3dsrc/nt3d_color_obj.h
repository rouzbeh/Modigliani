/** \file nt3d_color_obj.h - NT3D_color_o class declaration 
 * by Ahmed Aldo Faisal &copy; created  15.10.1998  
 */
/* NetTrader - finance management, analysis and simulation system 
 * Version:  0.2 						  
 * Copyright (C) 1998 Ahmed Aldo Faisal 			  
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


/* $Id: nt3d_color_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_color_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.6  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.5  2000/04/27 18:20:48  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.4  2000/04/27 09:57:08  face
 * switched to float based colors
 * texture is RGBA color value,
 * corrected nasty bug in nt3d_color_o, where the color bearing vector
 * was initialised as 3 and not as 4-vector (RGBA!)
 * :wq!
 *
 * Revision 1.3  1999/07/17 18:21:52  face
 * imodified
 *
 * Revision 1.2  1999/07/05 00:50:41  face
 * updated light color model
 *
 * Revision 1.1  1999/07/04 03:17:31  face
 * minor mods
 *
 *
 */

#ifndef _nt3d_color_h_
#define _nt3d_color_h_


#include "ntsrc/nt_main.h"
#include "ntsrc/nt_types.h"
#include "nt3d_obj.h"
#include "ntsrc/nt_vector_obj.h"

#include <GL/gl.h>

#include <valarray>




/** @short THe NT3D_color_o class handles the abstract color 
  concept. 
  \warning Once its derived classes are called in the drawing
  tree they affected ALL subsequent drawing (Modifies OpenGL state).  
  \bug unknown */
class NT3D_color_o : public NT_o {
public:
  NT3D_color_o();
  NT3D_color_o(GLfloat newR, GLfloat newG, GLfloat newB, GLfloat newAlpha);
  NT3D_color_o(const valarray<GLfloat> & newColorRGBA);
  //NT3D_color_o(const NT_vector_o & newColorRGBA);
  NT3D_color_o(const NT3D_color_o & original);
  const NT3D_color_o& operator= (const NT3D_color_o & right);
  ~NT3D_color_o();
  /*   Methods                */  
  virtual void Paint() const { glColor4fv(colorPtr);}
  void SetColor( GLfloat newR, GLfloat newG, GLfloat newB, GLfloat newAlpha);
  void SetColor( const GLfloat newColor[] );
  void SetColor( const NT3D_color_o & newColor);
  NTreal _R(){ return colorRGBA[0];}
  NTreal _G(){ return colorRGBA[1];}
  NTreal _B(){ return colorRGBA[2];}
  NTreal _Alpha(){ return colorRGBA[3];}
  NT3D_color_o operator+(const NT3D_color_o &right) const;
  NT3D_color_o operator-(const NT3D_color_o &right) const;
  NT3D_color_o operator*(NTreal alpha) const;
  /* return the pointer to the color Cache (for OpenGL draws) */
  const GLfloat * _colorPtr() {Update(); return (const GLfloat*) colorPtr;}
   /*   Data                   */  
protected:
  /*   Methods                */
  /*   Data                   */
private:
  /*   Methods                */  
  void Update();
  // 2DO implement range checking for input values
  /*   Data                   */
  valarray<GLfloat> colorRGBA;
  GLfloat* colorPtr;
};

extern NT3D_color_o& NT3D_alpha();
extern NT3D_color_o& NT3D_white();
extern NT3D_color_o& NT3D_red();
extern NT3D_color_o& NT3D_green();
extern NT3D_color_o& NT3D_blue();
extern NT3D_color_o& NT3D_yellow();
extern NT3D_color_o& NT3D_black();


#endif /* _nt3d_color_h_ */






