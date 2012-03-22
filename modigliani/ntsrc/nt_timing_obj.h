/* nt_timing_obj.h - measure timings with a stopwatch like class */
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


/* $Id: nt_timing_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_timing_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/05/18 10:38:33  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/05/13 09:42:57  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.1  1999/03/27 16:12:57  face
 * Initial revision
 *
 */

#ifndef _nt_timing_obj_h_
#define _nt_timing_obj_h_

#include <ctime>
// for clock() and CLOCKS_PER_SEC

#include "nt_main.h"
#include "nt_obj.h"
#include "nt_types.h"


/**  NT_timing_o a  stopwatch like measurement object:
        void    start()     : start timing
        double  stop()      : stop timing
        void    reset()     : set elapsed time to 0.0
        double  read()      : read elapsed time (in seconds)
*/




class NT_timing_o : public NT_o {
private:
    NTbool isRunning;
    NTdouble last_time;
    NTdouble total;

public:
    NT_timing_o() {
        reset();
        secs_per_tick = 1.0 / CLOCKS_PER_SEC;
    }
    void reset() {
        isRunning = false;
        last_time = 0.0;
        total=0.0;
    }
    void start() {
        if (!isRunning) {
            last_time = seconds();
            isRunning = true;
        }
    }
    NTdouble stop()  {
        if (isRunning)
        {
            total += seconds() - last_time;
            isRunning = false;
        }
        return total;
    }
    NTdouble read()   {
        if (isRunning)
        {
            total+= seconds() - last_time;
            last_time = seconds();
        }
        return total;
    }
    NTdouble seconds() {
        return ( (NTdouble) clock() ) * secs_per_tick;
    }
private:
    NTdouble secs_per_tick;
};



#endif /* _nt_timing_obh_h_ */
