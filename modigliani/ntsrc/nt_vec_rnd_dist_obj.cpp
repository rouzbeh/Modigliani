/**\file nt_vec_rnd_dist_obj.cpp - NT_vec_rnd_dist_o class implementation
 * by Ahmed Aldo Faisal &copy; created 26.6.1999
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

/* $Id: nt_vec_rnd_dist_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_vec_rnd_dist_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/04/27 23:52:45  face
 * IMPLEMENT nt_vector_o DESTRUCTOR
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.1  1999/06/26 16:49:02  face
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

#include "nt_vec_rnd_dist_obj.h"

/* ***      CONSTRUCTORS	***/

/** Create a NT_vec_rnd_dist_o */
NT_vec_rnd_dist_o::NT_vec_rnd_dist_o(NTsize newDim) {
	dim = newDim;
	rndDistPtrVec.resize(dim);
}

/* ***      COPY AND ASSIGNMENT	***/
NT_vec_rnd_dist_o::NT_vec_rnd_dist_o(
		const NT_vec_rnd_dist_o __attribute__((unused)) & original) :
		NT_o() {
}

const NT_vec_rnd_dist_o&
NT_vec_rnd_dist_o::operator=(const NT_vec_rnd_dist_o & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
NT_vec_rnd_dist_o::~NT_vec_rnd_dist_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */

NT_vector_o NT_vec_rnd_dist_o::RndVec() const {
	NT_vector_o tmp(3); //2DO speed up using cache unit
	NTsize ll;
	for (ll = 0; ll < dim; ll++) {
		tmp[ll] = (rndDistPtrVec[ll])->RndVal();
	}
	return tmp;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.4. */
