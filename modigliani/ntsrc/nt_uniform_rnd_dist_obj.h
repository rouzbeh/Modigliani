/* nt_uniform_rnd_dist_obj.h - Header for unfirom random distribution class */
/* by Ahmed A. Faisal, 15. 10. 1998(c) */
/*
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

/* $Id: nt_uniform_rnd_dist_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_uniform_rnd_dist_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/10/15 19:22:58  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/03 15:41:42  face
 * minor updates (made compatible to recent class name changes)
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.2  1999/06/26 16:49:02  face
 * *** empty log message ***
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.4  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/12/25 21:28:17  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.2  1998/10/21 23:55:11  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/10/19 14:09:03  face
 * Initial revision
 *
 */

#ifndef _nt_uniform_rnd_dist_obj_h_
#define _nt_uniform_rnd_dist_obj_h_

#include "nt_rnd_dist_obj.h"
#include "nt_vector_obj.h"

//
//	Class name : NT_uniform_rnd_dist_o
//
/**	NT_uniform_rnd_dist_o returns values out of a uniform
                    random distribution objects

	Idea : The usage idea is to asign the object to the desired variable
             either an NTreal or an NT_vector_o.
             The corresponding "type cast" will return random values
             in the data structure of requested type (implementing a true
             random "variable")

	       A dimension check is included to enhace tye safety
             HOWEVER, if necessary a typecast should be explicitly written:
             NT_uniform_rnd_dist_o uniform;
             float a = (NTreal) uniform;
	     */


class NT_uniform_rnd_dist_o : public NT_rnd_dist_o {
public:
    NT_uniform_rnd_dist_o();
    // NT_uniform_rnd_dist_o(NTint dim);
    NT_uniform_rnd_dist_o(NTreal lowerBound, NTreal upperBound);
    // NT_uniform_rnd_dist_o(NTreal lowerBound, NTreal upperBound, NTint dim);
    NT_uniform_rnd_dist_o(const NT_uniform_rnd_dist_o &);
    ~NT_uniform_rnd_dist_o();

    NTreal RndVal() const;
    operator NTreal() const;
    NTreturn ChangeBounds(NTreal lowerBound, NTreal upperBound);
    // operator NT_vector_o() const;

    NTreal _a() const {
        return a;
    };
    NTreal _b() const {
        return b;
    };
    NTreal _mean() const   {
        return mean;
    };
    NTreal _variance() const   {
        return variance;
    };
    // NTint _dimension() const   { return dimension; };
    friend ostream & operator<<(ostream & os, const NT_uniform_rnd_dist_o & self);

private:
    void set_statistics();
    NTreal a;
    NTreal b;
    NTreal interval;
    NTuint dimension;
    NTbool ab01; // faster-(0,1)-call flag
};


#endif /* _nt_uniform_rnd_dist_obj_h_ */


