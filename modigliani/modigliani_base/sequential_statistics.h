/**
 * @file sequential_statistics.h
 * @brief Sequential_statistics class header
 *
 * @author Ahmed Aldo Faisal &copy; created 5.11.2000
 * @version  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_SEQUENTIAL_STATISTICS_H_
#define MODIGLIANI_MODIGLIANI_BASE_SEQUENTIAL_STATISTICS_H_

#include <cmath>
#include <climits>
#include <cfloat>

#include "modigliani_base/types.h"
#include "modigliani_base/obj.h"

namespace modigliani_base {
/** @short Sequential_statistics class
\bug unknown
\warning unknown
*/
  class Sequential_statistics:public Obj {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Sequential_statistics();
    Sequential_statistics(const Sequential_statistics & original) = delete;
    const Sequential_statistics & operator=(const Sequential_statistics &
                                            right) = delete;
    virtual ~Sequential_statistics();

    void Reset();
    void Add(Real val);
    Size counter() const {
      return (counter_);
    }
    Real average() const {
      return (average_);
    }
    Real val_square_average() const {
      return (val_square_average_);
    }
    Real variance() const {
      return (variance_);
    }
    Real min() const {
      return (min_);
    }
    Real max() const {
      return (max_);
    }
    friend std::ostream & operator<<(std::ostream & str,
                                       const Sequential_statistics & self);
    /* ***  Data                 ** */
  protected:
    /* ***  Methods              ** */
    /* ***  Data                 ** */
  private:
    /* ***  Methods              ** */
     Real UpdateAverage(Real avg, Real val, Size n) const;
    /* ***  Data                 ** */
    Size counter_;
    Real average_;
    Real val_square_average_;
    Real variance_;
    Real min_;
    Real max_;
  };
}  // namespace modigliani_base
#endif  // MODIGLIANI_MODIGLIANI_BASE_SEQUENTIAL_STATISTICS_H_
