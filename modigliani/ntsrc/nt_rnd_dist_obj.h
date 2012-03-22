/* nt_rnd_dist_obj.h - Header for random distribution super class */
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


/* $Id: nt_rnd_dist_obj.h,v 1.4 2001/11/06 16:28:15 face Exp $
 * $Log: nt_rnd_dist_obj.h,v $
 * Revision 1.4  2001/11/06 16:28:15  face
 * *** empty log message ***
 *
 * Revision 1.3  2001/10/08 11:01:45  face
 * *** empty log message ***
 *
 * Revision 1.2  2001/10/03 14:37:13  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/10/15 19:22:58  face
 * *** empty log message ***
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
 * Revision 1.2  1998/10/20 14:51:52  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/10/19 14:09:03  face
 * Initial revision
 *
 */

#ifndef _nt_rnd_dist_obj_h_
#define _nt_rnd_dist_obj_h_

#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
#include "nt_vector_obj.h"

#include "nt_mersenne_twister_external_obj.h"




//
//	Class name : NT_rnd_dist_o
//
/**	NT_rnd_dist_o is a parent object to
                    individual random distribution objects.
	Idea : The usage idea is to call the object which returns
             a NTreal or NT_vector_o containing random values out
             of its specific distribution
	     */
class NT_rnd_dist_o : NT_o {
public:
    NT_rnd_dist_o();
    NT_rnd_dist_o(NTint dim);
    NT_rnd_dist_o(const NT_rnd_dist_o &);
    ~NT_rnd_dist_o();

    virtual NTreal RndVal() const = 0;
    //  NTreal NT_rnd_dist_o();
    // NT_vector_o NT_rnd_dist_o();

    NTreal _mean() const   {
        return mean;
    };
    NTreal _variance() const   {
        return variance;
    };

    friend ostream & operator<<(ostream & os, const NT_rnd_dist_o & self);

protected:
    float uniformQuickDirty() const;
    float uniform1() const;
    float uniformMT() const {
        return randgen.rand();
    }
    // 2DO: seed has to be mutexed !
    NTreal mean;
    NTreal variance;
private:
    static MTRand randgen;
    static long seed;
};




#endif /* _nt_rnd_dist_obj_h_ */




