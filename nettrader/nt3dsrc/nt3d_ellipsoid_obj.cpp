/* nt3d_ellipsoid_obj_obj.h - NT3D implementation for ellipsoid class
 * by Ahmed A. Faisal, 15. 10. 1998(c)
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


/* $Id: nt3d_ellipsoid_obj.cpp,v 1.1 2001/06/29 13:16:55 face Exp $
 * $Log: nt3d_ellipsoid_obj.cpp,v $
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/17 18:21:53  face
 * imodified
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
 * Revision 1.4  1998/12/25 21:26:30  face
 * new NetTrader version 0.2 set with this revision of this file new NetTrader version 0.2 set with this revision of this file
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


#include "nt3d_ellipsoid_obj.h"




/* Constructor/Destructor */


NT3D_ellipsoid_o::NT3D_ellipsoid_o()
        :
        oHalfAxis(1.0, 1.0, 1.0)
{
}

NT3D_ellipsoid_o::NT3D_ellipsoid_o(NT_vector3_o oNewTranslation, NT_vector3_o oNewHalfAxis)
{
    oTranslation = oNewTranslation;
    oHalfAxis = oNewHalfAxis;
}

/** \warning see corresponding note in NT3D_solid_o */
NT3D_ellipsoid_o::NT3D_ellipsoid_o(const NT3D_ellipsoid_o & original)
{
    oHalfAxis = original.oHalfAxis;
}


NT3D_ellipsoid_o::~NT3D_ellipsoid_o()
{
}

/* Public */
/** Draws a sphere scaled according to the oHalfAxis provided.
    Rotation in space has to be achived via oRotate of the parent object.
    It might be useful one day to add a rotation for the sphere object,
    without affecting the children model matrix */
void
NT3D_ellipsoid_o::DrawSolidObj() const
{
    glPushMatrix();
    glScalef(oScaling.x, oScaling.y, oScaling.z );
    glScalef(oHalfAxis.x, oHalfAxis.y, oHalfAxis.z );
    gluSphere( pQuadObj, 1.0, slices, stacks);
    glPopMatrix();
}




/* Protected */
/* Private */






















