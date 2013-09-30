/**
 * @file timing.h
 * @brief Measure timings with a stopwatch like class
 *
 * @author Ahmed A. Faisal, 15. 10. 1998(c)
 * @version  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENSE
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_TIMING_H_
#define MODIGLIANI_MODIGLIANI_BASE_TIMING_H_

#include <ctime>
// for clock() and CLOCKS_PER_SEC

#include "modigliani_base/main.h"
#include "modigliani_base/obj.h"
#include "modigliani_base/types.h"

namespace modigliani_base {

/**
 * @class Timing
 * @brief a stopwatch like measurement object.
 *
 */
  class Timing:public Obj {
  public:
     Timing() {
      reset();
      secs_per_tick_ = 1.0 / CLOCKS_PER_SEC;
    }

     /**
        @brief set elapsed time to 0.0
     */
    void reset() {
      is_running_ = false;
      last_time_ = 0.0;
      total_ = 0.0;
    }

    /**
       @brief Start timing
    */
    void start() {
      if (!isRunning) {
        last_time_ = seconds();
        is_running = true;
      }
    }

    /**
       @brief stop timing
       @return elapsed time in seconds
    */
    Real stop() {
      if (is_running_) {
        total += seconds() - last_time;
        isRunning = false;
      }
      return (total_);
    }

     /**
       @brief Read elapsed time in seconds
       @return elapsed time in seconds
    */
    Real read() {
      if (isRunning) {
        total += seconds() - last_time_;
        last_time_ = seconds();
      }
      return (total_);
    }

    Real seconds() {
      return (((Real) clock()) * secs_per_tick_);
    }

  private:
    Real secs_per_tick_;
    bool is_running_;
    Real last_time_;
    Real total_;
  };
}  // namespace modigliani_base

#endif  // MODIGLIANI_MODIGLIANI_BASE_TIMING_H_
