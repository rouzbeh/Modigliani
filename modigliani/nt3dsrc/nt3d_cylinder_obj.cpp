/* nt3d_cylinder_obj_obj.h - NT3D implementation for cylinder class
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

/* $Id: nt3d_cylinder_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_cylinder_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/04/27 23:52:27  face
 * further corrections
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
 * Revision 1.4  1999/02/17 17:46:18  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/11 02:26:40  face
 * Initial revision
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

#include "nt3d_cylinder_obj.h"
#include <math.h>

/* Constructor/Destructor */

NT3D_cylinder_o::NT3D_cylinder_o() :
		NT3D_solid_o() {
	Init(1.0);
}

NT3D_cylinder_o::NT3D_cylinder_o(NT_std::vector3_o oNewPosition) :
		NT3D_solid_o(oNewPosition) {
	Init(1.0);
}

NT3D_cylinder_o::NT3D_cylinder_o(NT_std::vector3_o oBaseCenter, mbase::Real newHeight,
		mbase::Real newRadius) :
		NT3D_solid_o(oBaseCenter) {
	Init(newRadius);
	height = newHeight;
	//  cerr << "NT3D_cylinder_o::NT3D_cylinder_o " <<oRotation[0] << " "<<oRotation[1] << " "<<oRotation[2] <<endl;
}

/** \warning see corresponding note in NT3D_solid_o */
NT3D_cylinder_o::NT3D_cylinder_o(const NT3D_cylinder_o & original) :
		NT3D_solid_o() {
	topRadius = original.topRadius;
	baseRadius = original.baseRadius;
	bHaveTopClosed = original.bHaveTopClosed;
	bHaveBaseClosed = original.bHaveBaseClosed;
	height = original.height;
}

NT3D_cylinder_o::~NT3D_cylinder_o() {
}

/* Public */
/**  */
inline void NT3D_cylinder_o::DrawSolidObj() const {
	glScalef(NT3D_solid_o::oScaling[0], NT3D_solid_o::oScaling[1],
			NT3D_solid_o::oScaling[2]);
	gluCylinder(pQuadObj, baseRadius, topRadius, height, slices, stacks);
}

/* Protected */
/* Private */
void NT3D_cylinder_o::Init(mbase::Real radius) {
	topRadius = radius;
	baseRadius = radius;
	height = 1.0;
	bHaveTopClosed = NT_FALSE;
	bHaveBaseClosed = NT_FALSE;
}

