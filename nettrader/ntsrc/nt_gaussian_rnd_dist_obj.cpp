/* nt_gaussian_rnd_dist_obj.cc - Methods for gaussian random distribution class */
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


/* $Id: nt_gaussian_rnd_dist_obj.cpp,v 1.2 2001/11/06 16:28:15 face Exp $
 * $Log: nt_gaussian_rnd_dist_obj.cpp,v $
 * Revision 1.2  2001/11/06 16:28:15  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/04/27 18:20:50  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
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
 * Revision 1.2  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.5  1998/10/21 23:55:07  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/10/20 14:51:45  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/10/20 10:18:55  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/10/20 07:53:28  face
 * .
 *
 * Revision 1.1  1998/10/19 14:09:00  face
 * Initial revision
 *
 */


#include "nt_gaussian_rnd_dist_obj.h"

#include <math.h>
NT_gaussian_rnd_dist_o::NT_gaussian_rnd_dist_o()
{
    mean = 0.0;
    stddevi = 1.0;
    stdNormSW = NT_TRUE;
}

NT_gaussian_rnd_dist_o::NT_gaussian_rnd_dist_o(NTreal newMean, NTreal newStddevi)
{
    NT_ASSERT(newStddevi >= 0.0);
    mean = newMean;
    stddevi = newStddevi;

    if ((0.0 == mean) && (1.0 == stddevi))  stdNormSW = NT_TRUE;
    else stdNormSW = NT_FALSE;
}



NT_gaussian_rnd_dist_o::~NT_gaussian_rnd_dist_o()
{
}



//
//	Method name : NT_gaussian_rnd_dist_o operator NTreal()
//
//	Description : "Cast" a random value to NTreal
//	Input : nothing
//	Output : an NTreal random value
//
NTreal
NT_gaussian_rnd_dist_o::RndVal() const
{
    if (NT_TRUE == stdNormSW) return (NTreal) gaussian01();
    else return (NTreal) gaussian01() * stddevi + mean;
}



//
//	Method name : gaussian01
//
//	Description : return gaussian random value with mean 0 and standard deviation (!) 1
//	Input :
//	Output : random value
//      Background : uses uniform1()
//
// See "float gasdev(long * idum)" from Numerical recipes in C, 2nd Edition, p. 289
//
float
NT_gaussian_rnd_dist_o::gaussian01() const
{
    static int iset = 0;
    static float gset;
    float fac, rsq, v1, v2;

    if (iset == 0) {
        do {
            v1 = 2.0 * uniformMT() - 1.0;
            v2 = 2.0 * uniformMT() - 1.0;
            rsq = v1*v1 + v2*v2;
        } while ( (rsq >= 1.0) || (0.0 == rsq) );
        fac = sqrt(-2.0 * log(rsq)/rsq);

        gset = v1*fac;
        iset = 1;

        return v2*fac;
    } else {
        iset = 0;
        return gset;
    }

}



