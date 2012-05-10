/* Timing.h - measure timings with a stopwatch like class */
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
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mbase_timing_h_
#define _mbase_timing_h_

#include <ctime>
// for clock() and CLOCKS_PER_SEC

#include "main.h"
#include "Obj.h"
#include "types.h"


/**  M_timing_o a  stopwatch like measurement object:
        void    start()     : start timing
        double  stop()      : stop timing
        void    reset()     : set elapsed time to 0.0
        double  read()      : read elapsed time (in seconds)
*/



namespace mbase{
class Timing : public Obj {
private:
    Mbool isRunning;
    Mdouble last_time;
    Mdouble total;

public:
    Timing() {
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
    Mdouble stop()  {
        if (isRunning)
        {
            total += seconds() - last_time;
            isRunning = false;
        }
        return (total);
    }
    Mdouble read()   {
        if (isRunning)
        {
            total+= seconds() - last_time;
            last_time = seconds();
        }
        return (total);
    }
    Mdouble seconds() {
        return (((Mdouble) clock() ) * secs_per_tick);
    }
private:
    Mdouble secs_per_tick;
};
}


#endif /* _mbase_timing_h_ */
