/**\file nt3d_sphere_obj.cpp - NT3D_sphere_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 17.7.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.4
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

/* $Id: nt3d_sphere_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: nt3d_sphere_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/07/06 08:06:10  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/06/26 19:14:06  face
 * finally found the error that caused solid object to disappear:
 * the scaling vector was not constructed in the 2nd constructor
 * of the solid bject thus resulting in a zero object
 * new demo file: showing some random graphics
 *
 * Revision 1.1  2000/06/13 20:10:26  face
 * *** empty log message ***
 *
 */

#include "nt3d_sphere_obj.h" 


/* ***      CONSTRUCTORS	***/
/** Create a NT3D_sphere_o */
NT3D_sphere_o::NT3D_sphere_o(const NT_vector3_o & oNewPosition, const NTreal newRadius )
:
NT3D_solid_o(oNewPosition),
radius(newRadius)
{
}


/* ***      COPY AND ASSIGNMENT	***/
NT3D_sphere_o::NT3D_sphere_o(const NT3D_sphere_o & original):
		NT3D_solid_o()
{
  oTranslation = original.oTranslation;
  radius = original.radius;
}

const NT3D_sphere_o&  
NT3D_sphere_o::operator= (const NT3D_sphere_o & right)
{
  if (this == &right) return *this; // Gracefully handle self assignment
  // add assignment code here
  oTranslation = right.oTranslation;
  radius = right.radius;
  return *this;
}


/* ***      DESTRUCTOR		***/
NT3D_sphere_o::~NT3D_sphere_o()
{
}

/* ***  PUBLIC                                    ***   */  


/* ***  PROTECTED                         ***   */
/** @short      Draw sphere
    @param      none
    @return     none
    \warning    
    \bug        unknown
 */
void
NT3D_sphere_o::DrawSolidObj() const
{
	glPushMatrix();

	NT_CERR(11,"NT3D_sphere_o::DrawSolidObj() called");
	glScalef(oScaling.x, oScaling.y, oScaling.z );
    gluSphere( pQuadObj, radius, slices, stacks);
    
	glPopMatrix(); 
}


/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.5. */
