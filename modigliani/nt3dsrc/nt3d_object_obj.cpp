/* nt3d_object_obj.cpp - NT 3D supcerclass implementation
 * by Ahmed A. Faisal, 5. 12. 1998(c) 
*
 * NetTrader - finance management, analysis and simulation system
 * Version:  0.1
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


/* $Id: nt3d_object_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_object_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/20 01:15:05  face
 * general debugging and clean up
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
 * Revision 1.7  1999/02/17 17:46:18  face
 * *** empty log message ***
 *
 * Revision 1.6  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.5  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/11 02:26:40  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/07 23:53:04  face
 * *** empty log message ***
 *
 * Revision 1.6  1999/01/07 12:56:15  face
 * *** empty log message ***
 *
 * Revision 1.5  1998/12/27 11:53:40  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/12/25 21:27:13  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.3  1998/12/25 11:03:07  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/16 14:57:08  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/06 15:03:10  face
 * Initial revision
 *
 */

#include "nt3d_object_obj.h"

#include "nt_error_obj.h"

#include <GL/gl.h>

/* Constructor/Destructor */

/** Sets a unrotated 3D object in the origin */
NT3D_object_o::NT3D_object_o()
:
  NT3D_o(),
  oTranslation(0.0,0.0,0.0),
  oRotation(0.0,0.0,0.0)
 {
 }

NT3D_object_o::NT3D_object_o(const NT_vector3_o & oNewTranslation)
 :
 oTranslation(oNewTranslation),
 oRotation(0.0,0.0,0.0)
 {
 }

/** The entire children tree is copied (recreated!) but no parent is
set, otherwise the parent of the original object has to be informed of
a new child which is generally not desired.  */
NT3D_object_o::NT3D_object_o(const NT3D_object_o & original)
:
NT3D_o()
{ 
  oTranslation = original.oTranslation;
  oRotation = original.oRotation;
}

 
NT3D_object_o::~NT3D_object_o()
{
}

/* Public */



void 
NT3D_object_o::Rotate(  const NT_vector3_o & oDeltaRotation )
{
oRotation = oRotation + oDeltaRotation;
}

void
NT3D_object_o::RotateX ( NTreal deltaRotX)
{
oRotation.x += deltaRotX;
}

void
NT3D_object_o::RotateY ( NTreal deltaRotY)
{
oRotation[1] += deltaRotY;
}

void
NT3D_object_o::RotateZ ( NTreal deltaRotZ)
{
oRotation[2] += deltaRotZ;
}

void
NT3D_object_o::RotateXYZ ( const NTreal deltaRot[3] )
{
oRotation.set_elem(deltaRot);
}


void
NT3D_object_o::Translate( const NT_vector3_o & oDeltaTranslation )
{
oTranslation = oTranslation + oDeltaTranslation;
}

void 
NT3D_object_o::TranslateX ( NTreal deltaTraX)
{
oTranslation.x += deltaTraX;
}

void 
NT3D_object_o::TranslateY ( NTreal deltaTraY)
{
oTranslation.y += deltaTraY;
}

void 
NT3D_object_o::TranslateZ ( NTreal deltaTraZ)
{
oTranslation.z += deltaTraZ;
}

void 
NT3D_object_o::TranslateXYZ ( const NTreal deltaTra[3] )
{
oTranslation.set_elem(deltaTra);
}



/* Private */








