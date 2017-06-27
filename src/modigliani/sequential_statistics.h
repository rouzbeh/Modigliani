/**
 * @file sequential_statistics.h
 * @brief Sequential_statistics class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_SEQUENTIAL_STATISTICS_H_
#define MODIGLIANI_MODIGLIANI_BASE_SEQUENTIAL_STATISTICS_H_

#include <cmath>
#include <climits>
#include <cfloat>

#include "modigliani/types.h"
#include "modigliani/obj.h"

namespace modigliani {
  /**
   * @brief Sequential_statistics class
   */
  class Sequential_statistics:public Obj {
 public:
    /**
     * @brief Create a Sequential_statistics
     */
    Sequential_statistics();
    
    Sequential_statistics(const Sequential_statistics & original) = delete;
    
    const Sequential_statistics & operator=(const Sequential_statistics &
                                            right) = delete;
    
    /**
     * @brief Desctructor
     */
    virtual ~Sequential_statistics();

    /**
     * @brief Resets all values
     */
    void Reset();

    /**
     * @brief Adds a value and update all statistics 
     * @param val Value to add
     */
    void Add(Real val);

    /**
     * @brief Returns number of samples
     * @return Number of sample
    */
    Size counter() const {
      return (counter_);
    }

    /**
     * @brief Returns the average
     * @return Average
    */
    Real average() const {
      return (average_);
    }

    /**
     * @brief Returns the sqaure average
     * @return Square average
    */
    Real val_square_average() const {
      return (val_square_average_);
    }

    /**
     * @brief Returns the variance
     * @return Variance
    */
    Real variance() const {
      return (variance_);
    }

    /**
     * @brief Returns the minimum value
     * @return Minimum value
    */
    Real min() const {
      return (min_);
    }

    /**
     * @brief Returns the maximum value
     * @return Maximum value
    */
    Real max() const {
      return (max_);
    }

    /**
     *  @brief Prints statistics
     *  @param  str ostream to output into
     *  @param  self
     *  @return ostream
     */
    friend std::ostream & operator<<(std::ostream & str,
                                     const Sequential_statistics & self);

 private:
    /**
     * @brief Returns the updated average value using the new sample
     * @param avg the current average
     * @param val the value
     * @param n size
     * @return The new average
     * @warning Does not update anything!
     */
    Real UpdateAverage(Real avg, Real val, Size n) const;

    Size counter_;
    Real average_;
    Real val_square_average_;
    Real variance_;
    Real min_;
    Real max_;
  };
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_BASE_SEQUENTIAL_STATISTICS_H_
