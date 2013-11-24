/**
 * @file timing.h
 * @brief Measure timings with a stopwatch like class
 *
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODIGLIANI_MODIGLIANI_BASE_TIMING_H_
#define MODIGLIANI_MODIGLIANI_BASE_TIMING_H_

// for clock() and CLOCKS_PER_SEC
#include <ctime>

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
     * @brief set elapsed time to 0.0
     */
    void reset() {
      is_running_ = false;
      last_time_ = 0.0;
      total_ = 0.0;
    }
    
    /**
     * @brief Start timing
     */
    void start() {
      if (!isRunning) {
        last_time_ = seconds();
        is_running = true;
      }
    }
    
    /**
     * @brief stop timing
     * @return elapsed time in [@f$\si{\second}@f$]
     */
    Real stop() {
      if (is_running_) {
        total += seconds() - last_time;
        isRunning = false;
      }
      return (total_);
    }

    /**
     * @brief Read elapsed time in seconds
     * @return elapsed time in [@f$\si{\second}@f$]
     */
    Real read() {
      if (isRunning) {
        total += seconds() - last_time_;
        last_time_ = seconds();
      }
      return (total_);
    }

    /**
     * @brief Returns elapsed time in seconds
     * @return Elapsed time in [@f$\si{\second}@f$]
     */
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
