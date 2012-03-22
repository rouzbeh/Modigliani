/* nt_uniform_rnd_dist_obj.cc - Methods for unfirom random distribution class */
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


/* $Id: nt_uniform_rnd_dist_obj.cpp,v 1.4 2001/11/06 16:28:15 face Exp $
 * $Log: nt_uniform_rnd_dist_obj.cpp,v $
 * Revision 1.4  2001/11/06 16:28:15  face
 * *** empty log message ***
 *
 * Revision 1.3  2001/10/12 09:18:39  face
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
 * Revision 1.2  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.3  1998/10/21 23:55:07  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/10/19 14:09:00  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/10/16 15:31:06  face
 * Initial revision
 *
 */

#include "nt_uniform_rnd_dist_obj.h"
#include <math.h>
#include <sys/time.h>
#include <unistd.h>



//
//	Method name : NT_uniform_rnd_dist_o
//
//	Description : CONSTRUCTORS
//	Input :
//	Output :
//

NT_uniform_rnd_dist_o::NT_uniform_rnd_dist_o()
{
    a = 0;
    b = 1;

// cerr <<"NT_uniform_rnd_dist_o::NT_uniform_rnd_dist_o() - Warning : RndVal() uses quick and dirty algortihm for 32 bit machienes and low period."<< endl;
    set_statistics();
}

NT_uniform_rnd_dist_o::NT_uniform_rnd_dist_o(NTreal lowerBound, NTreal upperBound)
{
    NT_ASSERT(lowerBound < upperBound);

    a = lowerBound;
    b = upperBound;

    set_statistics();
}


//
//	Method name : NT_uniform_rnd_dist_o
//
//	Description : DESTRUCTOR
//	Input :
//	Output :
//
NT_uniform_rnd_dist_o::~NT_uniform_rnd_dist_o()
{

}



inline NTreal
NT_uniform_rnd_dist_o::RndVal() const
{
    if (NT_TRUE == ab01) return (NTreal) uniformMT();
    else return (NTreal) ( interval*uniformMT() + a);
//	if (NT_TRUE == ab01) return (NTreal) uniformQuickDirty();
//    else return (NTreal) ( interval*uniformQuickDirty() + a);
}


//
//	Method name : NT_uniform_rnd_dist_o
//
//	Description : "Cast" a random value in (a,b) to NTreal
//	Input : nothing
//	Output : an NTreal random value in (a,b)
//
NT_uniform_rnd_dist_o::operator NTreal() const
{
    return RndVal();
}


NTreturn
NT_uniform_rnd_dist_o::ChangeBounds(NTreal lowerBound, NTreal upperBound)
{
    if (lowerBound >= upperBound) {
        cerr <<"NT_uniform_rnd_dist_o::ChangeBounds - Error : lowerBound "<<lowerBound
             <<"specified is equal or smaller than upperBound "<<upperBound<<". Bounds not changed." << endl;
        return NT_PARAM_OUT_OF_RANGE;
    }

    a = lowerBound;
    b = upperBound;

    set_statistics();
    return NT_SUCCESS;
}

//
//	Method nAMe : set_statistics
//
//	Description : set the statistical values according to (a,b),
//                    check whether to set the ab01-flag and compute the interval
//	Input : none
//	Output : none
//
void
NT_uniform_rnd_dist_o::set_statistics()
{
    //
    if (a == 0 && b == 1) ab01 = NT_TRUE;
    else ab01 = NT_FALSE;

    mean = (a + b) / 2.0;
    variance = fabs(a - b);

    interval = (fabs(a) + fabs(b));
}










