/**
 * @file Membrane.c++
 * Membrane class implementation
 * @author Ahmed Aldo Faisal &copy; created 16.3.2001  
 * @version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * @section LICENSE
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

/* $Id: ntbp_membrane_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_membrane_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#include "Membrane.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Membrane */
Membrane::Membrane() :
		Object() {
}
/* ***      COPY AND ASSIGNMENT	***/
Membrane::Membrane(const Membrane __attribute__((unused)) & original) :
		Object() {
	// add assignment code here
}

Membrane&
Membrane::operator=(const Membrane & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Membrane::~Membrane() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 
 mbase::Mreturn
 Membrane::Step()
 {
 std::cerr <<"Membrane::Step - ERROR : Not implemented." << std::endl;
 return mbase::M_NOT_IMPLEMENTED;
 }
 */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
