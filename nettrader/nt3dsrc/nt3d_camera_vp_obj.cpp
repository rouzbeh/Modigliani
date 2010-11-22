/**\file nt3d_camera_vp_obj.cpp - NT3D_camera_vp_o class implementation 
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


/* $Id: nt3d_camera_vp_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_camera_vp_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/10/01 09:33:47  face
 * *** empty log message ***
 *
 * Revision 1.6  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.5  2000/04/26 17:55:39  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.3  1999/07/20 01:14:58  face
 * general debugging and clean up
 *
 * Revision 1.2  1999/07/17 18:21:50  face
 * imodified
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.4  1999/06/26 16:49:02  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/06/19 16:15:49  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/06/06 23:51:34  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/05/13 11:34:53  face
 * Initial revision
 *
 * Revision 1.2  1999/05/09 09:34:15  face
 * corrected output
 *
 * Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
 *
 */

#include "nt3d_camera_vp_obj.h" 

/* ***      CONSTRUCTORS	***/

/** Create a NT3D_camera_vp_o */
NT3D_camera_vp_o::NT3D_camera_vp_o()
{
	haveCamera = false;
	SetCamera(new NT3D_camera_o);
}

NT3D_camera_vp_o::NT3D_camera_vp_o(NT3D_camera_o * newCameraPtr)
{
	haveCamera = false;
	SetCamera(newCameraPtr);
}

/* ***      COPY AND ASSIGNMENT	***/
/** copy constructor
The original pointer is used to create a new object .*/
NT3D_camera_vp_o::NT3D_camera_vp_o(const NT3D_camera_vp_o & original) 
{
	haveCamera = false;
	NT3D_camera_o * tmpCameraPtr = new NT3D_camera_o(*(original.cameraPtr));
	SetCamera(tmpCameraPtr);
}


/** operator=
The right hand side pointer is used to create a new object.
\warning The old cameraPtr is overwritten AND deleted. 
*/ 
const NT3D_camera_vp_o&  
NT3D_camera_vp_o::operator= (const NT3D_camera_vp_o & right)
{
  if (this == &right) return *this; // Gracefully handle self assignment
  // add assignment code here
  if (true == haveCamera) delete cameraPtr;
  SetCamera( new NT3D_camera_o(*(right.cameraPtr)));
  return *this;
}


/* ***      DESTRUCTOR		***/
/** destructor
\warning The cameraPtr is deleted, if existing. */
NT3D_camera_vp_o::~NT3D_camera_vp_o()
{
	if (true == haveCamera) delete cameraPtr;
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
    \warning    unknown
    \bug        unknown
 */
void
NT3D_camera_vp_o::Draw() 
{
  if (false == haveCamera) {
		cerr << "NT3D_universe_camera_vp_o::DrawScene - Camera was not added previously. Nothing drawn." <<endl;
		return;
  }
  MakeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cameraPtr->Draw();
  glPushMatrix();
    	DrawScene();
  glPopMatrix();
  SwapBuffer();
}


NTbool 
NT3D_camera_vp_o::SetCamera(NT3D_camera_o * newCameraPtr)
{
		if (NT_TRUE == haveCamera)
			NT_CERR(3,"NT3D_camera_vp_o::AddCamera - warning : new universe is overwriting old universe pointer.");
		cameraPtr = newCameraPtr;
		haveCamera = true;
		return NT_SUCCESS;

}



void 
NT3D_camera_vp_o::InitChild()
{
  //camera.Draw();	
  /* 2DO preliminary code *//*
  GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {0.0, 0.0, 20.0, 0.0};
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1); */
}

/* ***  PROTECTED                         ***   */
NTreturn 
NT3D_camera_vp_o::ResizeChild(NTsize width, NTsize height)
{
	cameraPtr->Set_aspectRatio(width/(NTreal)height);
	return NT_SUCCESS;
}



/* ***  PRIVATE                           ***   */
bool haveCamera;

/* File skeleton generated by GenNTObj version 0.4. */
