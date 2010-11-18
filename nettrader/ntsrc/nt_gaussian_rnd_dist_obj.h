/* nt_gaussian_rnd_dist_obj.h - Header for vectorial gaussian random distribution class */
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


/* $Id: nt_gaussian_rnd_dist_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_gaussian_rnd_dist_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/04/27 18:20:50  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.2  1999/07/03 15:41:40  face
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
 * Revision 1.6  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.5  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.5  1998/12/25 21:28:17  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.4  1998/10/21 23:55:11  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/10/20 14:51:52  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/10/20 10:18:59  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/10/19 14:09:03  face
 * Initial revision
 *
 */

#ifndef _nt_gaussian_rnd_dist_obj_h_
#define _nt_gaussian_rnd_dist_obj_h_

#include "nt_rnd_dist_obj.h"
#include "nt_vector_obj.h"

//
//	Class name : NT_gaussian_rnd_dist_o
//
/**	NT_gaussian_rnd_dist_o returns values out of a gaussian
                    random distribution objects based upon inidividual paramters
		      for each subspace
	Idea : The usage idea is to call the object which returns
             a NTreal or NT_vector_o containing random values out
             of its specific distribution [a,b]
	     */

class NT_gaussian_rnd_dist_o : public NT_rnd_dist_o {
public:
    NT_gaussian_rnd_dist_o();
    NT_gaussian_rnd_dist_o(NTreal newMean, NTreal newStdDev);
    NT_gaussian_rnd_dist_o(const NT_gaussian_rnd_dist_o &);
    virtual ~NT_gaussian_rnd_dist_o();
    operator NTreal() const {
        return RndVal();
    }
    NTreal RndVal() const;
    NTreal _stddevi() const  {
        return stddevi;
    };
    friend ostream & operator<<(ostream & os, const NT_gaussian_rnd_dist_o & self);


protected:
    float gaussian01() const;

private:
    //! switch for faster standard normal distribution computation
    NTbool stdNormSW;
    NTreal stddevi;
};





#endif /* _nt_gaussian_rnd_dist_obj_h_ */

