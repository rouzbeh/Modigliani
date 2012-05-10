/** \file Gui_drv.c++ - Gui_drv class implementation
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
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Gui_drv.h"

using namespace mbase;

/* 	CONSTRUCTORS 		*/
/** Create a gui_drv from origin (0,0,0) to (1,1,1) */
Gui_drv::Gui_drv() {
}

/* 	COPY AND ASSIGNMEM 	*/
Gui_drv::Gui_drv(
		const Gui_drv __attribute__((unused)) & original) :
		Obj() {
}

const Gui_drv&
Gui_drv::operator=(const Gui_drv & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* 	DESTRUCTOR 		*/
Gui_drv::~Gui_drv() {
}

/* *** 	PUBLIC				          ***	*/
/** @short
 @param 	none
 @return 	none
 \warning 	unknown
 \bug 	unknown
 */
void Gui_drv::SwapBuffer() const {
}

void Gui_drv::MakeCurrent() const {
}

void Gui_drv::Resize(Msize __attribute__((unused)) width,
		Msize __attribute__((unused)) height) {
}

void Gui_drv::Redraw() {
}
/* *** 	PROTECTED     	      		  ***	*/
/* ***	PRIVATE               		  ***   */

