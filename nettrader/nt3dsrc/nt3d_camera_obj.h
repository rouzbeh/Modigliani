/**\file nt3d_camera_obj.h - NT3D_camera_o class header 
 * by Ahmed Aldo Faisal &copy; created 22.4.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.3
 * Copyright (C) 1998,199 Ahmed Aldo Faisal                            
 *        camera.SetTranslationX(15*cos(t));
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

/* $Id: nt3d_camera_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_camera_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/10/01 09:33:47  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/04/27 18:20:48  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.6  2000/04/26 17:55:39  face
 * *** empty log message ***
 *
 * Revision 1.5  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.4  1999/07/17 18:21:49  face
 * imodified
 *
 * Revision 1.3  1999/07/05 00:50:39  face
 * updated light color model
 *
 * Revision 1.2  1999/07/03 16:25:24  face
 * parametrised fov
 * access method still necessary
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.5  1999/06/19 16:15:49  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/05/13 11:34:59  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/05/08 23:58:55  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/04/24 15:02:58  face
 * 0.3 .
 *
 * Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
 */

#ifndef _nt3d_camera_h_
#define _nt3d_camera_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include "nt3d_object_obj.h"

#include "nt_vector3_obj.h"
/** @short NT3D_camera_o class
The class provides a freely movable imaginary camera
in a three dimensional universe. 

Idea: Its position in space is
determined by the translation vector of the parent class.
The camera is rotated and moved like any NT3D_object_o object,
where the rotation describes the direction in which the imaginary
lens is pointed (default the positive X axis). Under all rotations
the upper border of the camera "picture" is parallel to a predefined direction
(default is the positive Y axis). 

Usage: The camera abtraction is based on a appropriate transformation
of the projection and the model view matrices. Thus, if rendering
should refelct what the camera "see´s" NT3D_camera_o.Draw() should be invoked
before the universe is drawn.
\bug unknown
\warning unknown 
 */
class NT3D_camera_o : public NT3D_object_o {
public:
  /***   Constructors, CopyAssignment and Destructor  ***/  
  NT3D_camera_o(const NT_vector3_o & oNewPosition = NT_OriginVec3());
  NT3D_camera_o(const NT_vector3_o & oNewPosition, const NT_vector3_o & oNewTarget);
  NT3D_camera_o(const NT3D_camera_o & original);
  const NT3D_camera_o & operator= (const NT3D_camera_o & right);
  virtual ~NT3D_camera_o();
  /* ***  Methods              ***/  
  /** set the camera position, i.e. set the projection and viewing transformation, 
   ** The method is virtual such that derived classes can overwrite it */
  virtual void Draw();
  NTreal _aspectRatio() const{return aspectRatio;}
  void Set_aspectRatio(NTreal newRatio){aspectRatio = newRatio;}
  void SetTarget( const NT_vector3_o & oNewTarget ) { oTarget=oNewTarget;} 
  NT_vector3_o _oTarget() {return oTarget; }
 /* ***  Data                 ***/  
  protected:
  /* ***  Methods              ***/  
  void ControlImpl(NT3Dcontroller cntrl , NTint value){};
  void ControlImpl(NT3Dcontroller cntrl , NTreal value){}; 
  void ControlImpl(NT3Dcontroller cntrl , NT_vector_o value){};
  /** change the value of the aspectRatio, not observable until Draw() call */
  /* ***  Data                 ***/  
  /** where to look at */
  NT_vector3_o oTarget;
  /** where the up, i.e. the upper edge of the camera "picture" is */ 
  NT_vector3_o oUp;
  /* was the camera object changed since the last Draw() ? */
  bool cameraChanged;  
  /** The ascpect ratio determines the ratio of width to height of
     the "film" which is recorded from the camera. It usually corresponds
     to the aspect ratio of a (rendering) viewport. */
  NTreal aspectRatio;  
  /** The camera Field Of View (FOV) is an angle in ]0,180[ */
  NTreal fov;

  private:
  /* ***  Methods              ***/
  /* ***  Data                 ***/
};


#endif /* _nt3d_camera_h_ */

/* File skeleton generated by GenNTObj version 0.3. */
