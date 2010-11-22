/** \file nt3d_line_obj.h - NT3D_line_o class declaration 
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


/* $Id: nt3d_line_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_line_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.2  1999/07/04 03:17:32  face
 * minor mods
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 1.1  1999/04/20 13:47:34  face
 * Initial revision
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.3  1999/01/18 01:02:26  face
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

#ifndef _nt3d_line_h_
#define _nt3d_line_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt3d_hierarchic_object_obj.h"
#include "nt3d_material_obj.h"
#include "nt_vector3_obj.h"

/** @short NT3D_line_o class
  Line representation between to arbitraty positions in space.

  The object uses a glBegin(GL_LINES)  glEnd call to draw the line
  the line in three dimension.
\warning unknown 
\bug unknown
 */
class NT3D_line_o : public NT3D_hierarchic_object_o 
{
public:
  NT3D_line_o();
  NT3D_line_o(NT_vector3_o newStartPnt, NT_vector3_o newEndPnt);
  NT3D_line_o(const NT3D_line_o & original);
  const NT3D_line_o& operator= (const NT3D_line_o & right);
  ~NT3D_line_o();
  /*   Methods                */  
  void DrawObj();
  /*   Data                   */  
  NT3D_material_o oMaterial;
  protected:
  /*   Methods                */  
  /*   Data                   */  
  private:
  /*   Methods                */  
  void ControlObj(NT3Dcontroller cntrl , NTint value) {};
  void ControlObj(NT3Dcontroller cntrl , NTreal value) {}; 
  void ControlObj(NT3Dcontroller cntrl , NT_vector_o value){};
  /*   Data                   */
  NT_vector3_o startPnt;
  NT_vector3_o endPnt;
};




#endif /* _nt3d_line_h_ */






