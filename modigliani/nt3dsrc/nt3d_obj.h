/** \file nt3d_obj.h - NT3D_o class declaration 
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


/* $Id: nt3d_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/04/28 16:02:40  face
 * found nasty hidden bug, by initialisation of an hideen ellipsoid making the
 * entire program crash
 * on destructor call... reason unknown
 *
 * Revision 1.3  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.2  1999/07/04 03:17:33  face
 * minor mods
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.1  1999/04/20 13:47:34  face
 * Initial revision
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.3  1999/02/17 17:46:22  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/11 02:26:45  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/07 23:52:56  face
 * Initial revision
 *
 * Revision 1.1  1998/12/27 11:53:34  face
 * Initial revision
 *
 *
 */

#ifndef _nt3d_h_
#define _nt3d_h_


#include "ntsrc/Obj.h"

#include <GL/gl.h>
#define NT3D_MAX_LIGHTS GL_MAX_LIGHTS
#define NT3D_MAX_TEXTURES 16

/** @short NT3D_o class
  The superclass for the NT3D graphic visualisation classes 
\warning unknown 
\bug unknown
 */
class NT3D_o : public mbase::Obj {
public:
  /*   Methods                */  
  virtual void Draw() = 0;
  /*   Data                   */  
  protected:
  /*   Methods                */  
  /*   Data                   */  
  private:
  /*   Methods                */  
  /*   Data                   */
};




#endif /* _nt3d_h_ */






