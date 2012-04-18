/** \file nt_gui_drv_obj.cpp - NT gui_drv class implementation
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

/* $Id: nt_gui_drv_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_gui_drv_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.2  1999/03/22 01:01:04  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/03/21 16:22:52  face
 * Initial revision
 *
 *
 */

#include "nt_gui_drv_obj.h"

/* 	CONSTRUCTORS 		*/
/** Create a gui_drv from origin (0,0,0) to (1,1,1) */
NT_gui_drv_o::NT_gui_drv_o() {
}

/* 	COPY AND ASSIGNMENT 	*/
NT_gui_drv_o::NT_gui_drv_o(
		const NT_gui_drv_o __attribute__((unused)) & original) :
		NT_o() {
}

const NT_gui_drv_o&
NT_gui_drv_o::operator=(const NT_gui_drv_o & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* 	DESTRUCTOR 		*/
NT_gui_drv_o::~NT_gui_drv_o() {
}

/* *** 	PUBLIC				          ***	*/
/** @short
 @param 	none
 @return 	none
 \warning 	unknown
 \bug 	unknown
 */
void NT_gui_drv_o::SwapBuffer() const {
}

void NT_gui_drv_o::MakeCurrent() const {
}

void NT_gui_drv_o::Resize(NTsize __attribute__((unused)) width,
		NTsize __attribute__((unused)) height) {
}

void NT_gui_drv_o::Redraw() {
}
/* *** 	PROTECTED     	      		  ***	*/
/* ***	PRIVATE               		  ***   */

