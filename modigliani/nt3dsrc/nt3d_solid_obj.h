/** \file nt_solid_obj.h - NT_solid_o class declaration 
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


/* $Id: nt3d_solid_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_solid_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.13  2000/07/06 08:06:10  face
 * *** empty log message ***
 *
 * Revision 1.12  2000/06/26 19:14:06  face
 * finally found the error that caused solid object to disappear:
 * the scaling vector was not constructed in the 2nd constructor
 * of the solid bject thus resulting in a zero object
 * new demo file: showing some random graphics
 *
 * Revision 1.11  2000/06/13 20:10:26  face
 * *** empty log message ***
 *
 * Revision 1.10  2000/05/18 10:38:25  face
 * an amazing error sits in nt3d_file_texture
 * if the constructor constructs a texture(1,1,1)
 * the system does not display the texture
 * if (64,64,2) is specified it does.
 * this is strange because the load routinge
 * should adapt the object correctly...investigate here 2DO
 *
 * Revision 1.9  2000/05/13 09:42:55  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Version 0.5)
 * rearranged color and material classes...
 * texture class added
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
 * Revision 1.5  2000/04/26 22:57:45  face
 * added new classes with rainbow_texture as demo class
 *
 * Revision 1.4  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.3  1999/07/20 01:15:07  face
 * general debugging and clean up
 *
 * Revision 1.2  1999/07/04 03:17:36  face
 * minor mods
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.5  1999/06/23 14:23:20  face
 * *** empty log message ***
 *
 * Revision 2.4  1999/06/06 23:51:28  face
 * *** empty log message ***
 *
 * Revision 2.3  1999/05/13 /usr/lib/Mesa/samples/11:34:59  face
 * *** empty log message ***
 *
 * Revision 2.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.5  1999/02/17 17:46:22  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/11 02:26:45  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/07 23:52:56  face
 * Initial revision
 *
 * Revision 1.1  1998/12/25 21:28:17  face
 * Initial revision
 *
 *
 */

#ifndef _nt3d_solid_h_
#define _nt3d_solid_h_

#include "ntsrc/nt_main.h"
#include "ntsrc/nt_types.h"
#include "nt3d_surfaced_object_obj.h"
#include "nt3d_material_obj.h"
#include "nt3d_texture_obj.h"

#include <GL/gl.h>
#include <GL/glu.h>

/** @short NT3D_solid_o class
  ...
\bug unknown
\warning oTexturePtr is deleted by destructor 
 */
class NT3D_solid_o : public NT3D_surfaced_object_o {
public:
  NT3D_solid_o(const NT_vector3_o & oNewPosition = NT_OriginVec3());
  NT3D_solid_o(const NT3D_solid_o & original);
  const NT3D_solid_o & operator= (const NT3D_solid_o & right);
  ~NT3D_solid_o();
  /*   Methods                */  
  void Scale( NT_vector3_o oDeltaScaling );
  void ScaleX ( NTreal deltaSclX);
  void ScaleY ( NTreal deltaSclY);
  void ScaleZ ( NTreal deltaSclZ);
  void ScaleXYZ ( NTreal deltaSclXYZ );
  void ScaleUnit(){ oScaling = NT_vector3_o(1.0,1.0,1.0);}
  
  /*   Data                   */  
  protected:
  /*   Methods                */  
  void DrawSurfacedObj();
  virtual void DrawSolidObj() const {NT_CERR(1,"NT3D_solid_o::DrawSolidObj() - Error : This method should be overridden by a derived class.");};
  void ControlSurfacedObj(NT3Dcontroller cntrl , NTint value);
  /*   Data                   */  
  /** pointer to the incomplete type of an GLUquadicObj */
  GLUquadricObj * pQuadObj;
  GLint slices;
  GLint stacks;
  NT_vector3_o oScaling;
  NTint drawStyle;
  private:
  /*   Methods                */  
  void InitQuadric();
 
  /*   Data                   */
};




#endif /* _nt3d_solid_h_ */









