/**\file nt3d_camera_vp_obj.h - NT3D_camera_vp_o class header 
 * by Ahmed Aldo Faisal &copy; created 9.5.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.3
 * Copyright (C) 1998,199 Ahmed Aldo Faisal                            
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

/* $Id: nt3d_camera_vp_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_camera_vp_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.6  2000/10/01 09:33:47  face
 * *** empty log message ***
 *
 * Revision 1.5  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.3  1999/07/20 01:14:59  face
 * general debugging and clean up
 *
 * Revision 1.2  1999/07/16 15:05:04  face
 * added code::^
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.3  1999/06/19 16:15:49  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/06/06 23:51:28  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/05/13 11:34:59  face
 * Initial revision
 *
 * Revision 1.2  1999/05/09 09:34:15  face
 * corrected output
 *(
 * Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
*/
#ifndef _nt3d_camera_vp_h_
#define _nt3d_camera_vp_h_


#include "mbase/nt_main.h"
#include "mbase/nt_types.h"
#include "mbase/nt_obj.h"

#include "nt3d_abstract_vp_obj.h"
#include "nt3d_camera_obj.h"
/** @short NT3D_camera_vp_o class
 Provides a viewport with a universe rendered though the view of an camera abstraction
\bug unknown
\warning do not override Draw() use DrawScene() instead!
 */
class NT3D_camera_vp_o : public NT3D_abstract_vp_o {
public:
  /***   Constructors, CopyAssignment and Destructor  ***/  
  NT3D_camera_vp_o();
  NT3D_camera_vp_o(NT3D_camera_o * newCameraPtr);
  NT3D_camera_vp_o(const NT3D_camera_vp_o & original);
  const NT3D_camera_vp_o & operator= (const NT3D_camera_vp_o & right);
  virtual ~NT3D_camera_vp_o();
  /* ***  Methods              ***/  
  void Draw();
  mbase::Mbool SetCamera(NT3D_camera_o * newCameraPtr);
  mbase::Mbool _hasCamera() { return haveCamera;}
  void RotateX ( const mbase::Mreal deltaRotX){ cameraPtr->RotateX(deltaRotX);}
  void RotateY ( const mbase::Mreal deltaRotY){ cameraPtr->RotateY(deltaRotY);}
  void RotateZ ( const mbase::Mreal deltaRotZ){ cameraPtr->RotateZ(deltaRotZ);}
  void SetRotationX ( const mbase::Mreal rotX){ cameraPtr->SetRotationX(rotX); }
  void SetRotationY ( const mbase::Mreal rotY){ cameraPtr->SetRotationY(rotY); }
  void SetRotationZ ( const mbase::Mreal rotZ){ cameraPtr->SetRotationZ(rotZ); }
  void TranslateX ( const mbase::Mreal deltaTraX){ cameraPtr->TranslateX(deltaTraX);}
  void TranslateY ( const mbase::Mreal deltaTraY){ cameraPtr->TranslateY(deltaTraY);}
  void TranslateZ ( const mbase::Mreal deltaTraZ){ cameraPtr->TranslateZ(deltaTraZ);}
  void SetTranslationX ( const mbase::Mreal traX){ cameraPtr->SetTranslationX(traX); }
  void SetTranslationY ( const mbase::Mreal traY){ cameraPtr->SetTranslationY(traY); }
  void SetTranslationZ ( const mbase::Mreal traZ){ cameraPtr->SetTranslationZ(traZ); }
  void SetTranslationXYZ ( const NT_std::vector3_o & oTra){ cameraPtr->SetTranslationXYZ(oTra); }
  void SetTarget ( const NT_std::vector3_o & oTarget){ cameraPtr->SetTarget(oTarget); }

  
  /* ***  Data                 ***/  

protected:
  /* ***  Methods              ***/  
  virtual void DrawScene() = 0;
  void RedrawChild(){DrawScene();};
  mbase::Mreturn ResizeChild(mbase::Msize width, mbase::Msize height);
  void InitChild() ;
  /* ***  Data                 ***/  
  NT3D_camera_o * cameraPtr;

private:
  /* ***  Methods              ***/  
  /* ***  Data                 ***/	
  bool haveCamera;
  
  };
#endif /* _nt3d_camera_vp_h_ */ 

/* File skeleton generated by GenNTObj version 0.4. */
