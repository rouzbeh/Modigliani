/** \file nt3d_line_obj.cpp - NT template2 class implementation
 * by Ahmed Aldo Faisal &copy; created 15.10.1998
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:   0.2
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


/* $Id: nt3d_line_obj.cpp,v 1.1 2001/06/29 13:16:55 face Exp $
 * $Log: nt3d_line_obj.cpp,v $
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.5  2000/06/26 19:14:06  face
 * finally found the error that caused solid object to disappear:
 * the scaling vector was not constructed in the 2nd constructor
 * of the solid bject thus resulting in a zero object
 * new demo file: showing some random graphics
 *
 * Revision 1.4  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.3  2000/04/27 18:20:48  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.2  1999/07/04 10:57:10  face
 * modified from rainbow to color object
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
 * Revision 1.5  1999/02/17 17:46:18  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/11 02:26:40  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/07 23:53:04  face
 * Initial revision
 *
 * Revision 1.2  1999/01/07 12:56:15  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/27 11:53:40  face
 * Initial revision
 *
 *
 */

#include "nt3d_line_obj.h"

#include <GL/gl.h>

/* 	CONSTRUCTORS 		*/
/** Create a line from origin (0,0,0) to (1,1,1) */
NT3D_line_o::NT3D_line_o()
{
    startPnt = NT_OriginVec3();
    endPnt = NT_UnitXYZVec3();
}

NT3D_line_o::NT3D_line_o(NT_vector3_o newStartPnt, NT_vector3_o newEndPnt)
{
    NT_ASSERT( 3 == newStartPnt._dimension());
    NT_ASSERT( 3 == newEndPnt._dimension());
    startPnt = newStartPnt;
    endPnt = newEndPnt;
}


/* 	COPY AND ASSIGNMENT 	*/
NT3D_line_o::NT3D_line_o(const NT3D_line_o & original)
        :
        NT3D_hierarchic_object_o(original),
        oMaterial(original.oMaterial)
{
    startPnt = original.startPnt;
    endPnt = original.endPnt;
}

const NT3D_line_o&
NT3D_line_o::operator= (const NT3D_line_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
    // add assignment code here
    startPnt = right.startPnt;
    endPnt = right.endPnt;
    return *this;
}


/* 	DESTRUCTOR 		*/
NT3D_line_o::~NT3D_line_o()
{
}


/*   	PUBLIC                	*/
/** @short 	draw the line
    @param 	none
    @return 	none
    \warning 	unknown
    \bug 	unknown
 */
void
NT3D_line_o::DrawObj()
{
    oMaterial.Paint();
    glBegin(GL_LINES);
    glVertex3dv ( startPnt._elem() );
    glVertex3dv ( endPnt._elem() );
    glEnd();
    //glFlush(); //2DO is the flush really needed ?
}


/*   	PROTECTED     	      	*/


/* 	PRIVATE               	*/










