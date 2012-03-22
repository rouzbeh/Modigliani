/* nt_rnd_dist_obj.cc - Methods for random distribution super class */
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


/* $Id: nt_rnd_dist_obj.cpp,v 1.4 2001/11/06 16:28:15 face Exp $
 * $Log: nt_rnd_dist_obj.cpp,v $
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
 * Revision 1.2  1999/01/07 23:53:04  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/25 21:27:13  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.2  1998/10/20 14:51:45  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/10/19 14:09:00  face
 * Initial revision
 *
 */

#include "nt_rnd_dist_obj.h"

long NT_rnd_dist_o::seed = (long) time(NULL); // 2DO improve initialization of seed
MTRand NT_rnd_dist_o::randgen = NTMakeMTRandFunction( MTRand::uint32(~time(NULL)) );



NT_rnd_dist_o::NT_rnd_dist_o()
{
    if ( (1.0 - 1.2e-7) == 1.0 ) cerr << "NT_rnd_dist_o::NT_rnd_dist_o - Error : Insufficient floating point number accuracy (1.0 - 1.2e-7) == 1.0  in this implementation " << endl;
}


NT_rnd_dist_o::~NT_rnd_dist_o() {}



/** See "float ranqd2()" from Numerical recipes in C, 2nd Edition, p. 284 */
/** WARNING requires IEEE 32 bit representation of floats */
inline float
NT_rnd_dist_o::uniformQuickDirty() const
{
    static unsigned long jflone = 0x3f800000;
    static unsigned long jflmsk = 0x007fffff;

    static unsigned long idum;
    static unsigned long itemp;
    float randval;

    idum = 1664525L*idum + 1013904223L;
    itemp = jflone | (jflmsk & idum);
    randval = (*(float *)&itemp)-1.0;

    return randval;
}


//
//	Method name : uniform1
//
/**      return uniform random values within (0,1)
	Input : initialize idum once(!) to a negative integer
	Output : random value within (0,1)
      Background : Long period ( > 2 x 10^18 ) generator of L�Ecuyer with Bays-Durham
                   shuffle and added safeguards
      Note: (1.0-1.2e-7) should approach the smallest number below 1.0

 See "float ran2(long * idum)" from Numerical recipes in C, 2nd Edition, p. 282
 */
inline float
NT_rnd_dist_o::uniform1() const
{
    int j;
    long k;
    static long idum2=123456789;
    static long iy=0;
    static long iv[32];
    float temp;

    // cerr << "NT_uniform_rnd_dist_o the seed is " << seed << endl;

    // init
    if (seed <= 0) {
        // prevent seed = 0
        if (-seed < 1)  seed = 1;
        else seed = -seed;
        idum2 = seed;
        // load shuffle table after 8 warm-up loops
        for ( j = 39; j >=0; j--) {
            k = seed / 53668;
            seed=40014*(seed-k*53668)-k*12211;
            if (seed < 0) seed += 2147483563;
            if (j < 32) iv[j] = seed;
        }
        iy = iv[0];
    }
    // running mode (after init)
    k=seed/53668;
    // execute the two random numbers
    // we use Schrage�s method two avoid overflow on 32-bit machienes
    seed=40014*(seed -k * 53668) - k * 12211;
    if (seed < 0) seed += 2147483563;
    k = idum2 / 	52774;
    idum2 = 40692 * (idum2 - k * 52774) -k * 3791;
    if (idum2 < 0) idum2 += 2147483399;
    // the shuffle table index j will be always in 0..31 (proved)
    j = iy/ (1+2147483562/32);
    // shuffle the seed, combine seed and idum2
    iy = iv[j] -idum2;
    iv[j] = seed;
    if (iy < 1) iy += 2147483562;
    // exclude endpoint value 1.0, so  ____________ should approach 1.0 -EPS != 1.0
    if ((temp= (1.0/2147483563)*iy) >  (1.0-1.2e-7)) return  (1.0-1.2e-7);
    else return temp;
}








