/** \file nt3D_arrow_obj.cpp - NT arrow class implementation
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


/* $Id: nt3d_arrow_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_arrow_obj.cpp,v $
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
 * Revision 1.1  1999/04/20 13:47:34  face
 * Initial revision
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.1  1999/02/17 17:46:18  face
 * Initial revision
 *
 *
 */

#include "nt3d_arrow_obj.h"


/* 	CONSTRUCTORS 		*/
/** Create a arrow from origin (0,0,0) to (1,1,1) */
NT3D_arrow_o::NT3D_arrow_o()
{
}


/* 	COPY AND ASSIGNMENT 	*/
NT3D_arrow_o::NT3D_arrow_o(const NT3D_arrow_o & original)
{
}

const NT3D_arrow_o&  
NT3D_arrow_o::operator= (const NT3D_arrow_o & right)
{
  if (this == &right) return *this; // Gracefully handle self assignment
  // add assignment code here
  return *this;
}


/* 	DESTRUCTOR 		*/
NT3D_arrow_o::~NT3D_arrow_o()
{
}


/* *** 	PUBLIC				          ***	*/  
/** @short 	 
    @param 	none
    @return 	none
    \warning 	unknown
    \bug 	unknown
 */
void 
NT3D_arrow_o::Collinearize(const NT_vector3_o &)
{
}


/* *** 	PROTECTED     	      		  ***	*/
/* ***	PRIVATE               		  ***   */










