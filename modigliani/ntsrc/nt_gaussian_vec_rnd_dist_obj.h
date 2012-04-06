/**\file nt_gaussian_vec_rnd_dist_obj.h - NT_gaussian_vec_rnd_dist_o class header
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

/* $Id: nt_gaussian_vec_rnd_dist_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_gaussian_vec_rnd_dist_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
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
*/
#ifndef _nt_gaussian_vec_rnd_dist_h_
#define _nt_gaussian_vec_rnd_dist_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include "nt_vec_rnd_dist_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"

/** @short NT_gaussian_vec_rnd_dist_o class

\bug unknown
\warning unknown
 */
class NT_gaussian_vec_rnd_dist_o : public NT_vec_rnd_dist_o {
public:
    /***   Constructors, CopyAssignment and Destructor  ***/
    NT_gaussian_vec_rnd_dist_o(NTreal mean, NTreal stdDev, NTsize dim);
    NT_gaussian_vec_rnd_dist_o(NT_vector_o mean, NT_vector_o stdDev, NTsize dim);

    NT_gaussian_vec_rnd_dist_o(const NT_gaussian_vec_rnd_dist_o & original);
    const NT_gaussian_vec_rnd_dist_o & operator= (const NT_gaussian_vec_rnd_dist_o & right);
    virtual ~NT_gaussian_vec_rnd_dist_o();
    /* ***  Methods              ***/
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    NT_vector_o meanVec;
    NT_vector_o stdDevVec;
};
#endif /* _nt_gaussian_vec_rnd_dist_h_ */

/* File skeleton generated by GenNTObj version 0.4. */