/**\file nt3d_vp_spec_obj.cpp - NT3D_vp_spec_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 10.6.1999  
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

/* $Id: nt3d_vp_spec_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_vp_spec_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.1  1999/06/19 16:15:49  face
 * Initial revision
 *
 * Revision 1.3  1999/06/06 23:52:06  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/05/09 09:34:15  face
 * corrected output
 *
 * Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
 *
 */

#include "nt3d_vp_spec_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_vp_spec_o */
NT3D_vp_spec_o::NT3D_vp_spec_o() {
	width = 200;
	height = 200;
	windowName = "Unnamed Window";
}

/* ***      COPY AND ASSIGNMENT	***/
NT3D_vp_spec_o::NT3D_vp_spec_o(const NT3D_vp_spec_o & original) :
		NT_o() {
	width = original.width;
	height = original.height;
	windowName = original.windowName;
}

const NT3D_vp_spec_o&
NT3D_vp_spec_o::operator=(const NT3D_vp_spec_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_vp_spec_o::~NT3D_vp_spec_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 
 void
 NT3D_vp_spec_o::() const
 {
 }
 */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.4. */