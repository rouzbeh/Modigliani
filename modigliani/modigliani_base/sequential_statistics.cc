/**
 * @file sequential_statistics.cc
 * @brief Sequential_statistics class implementation
 * @author Ahmed Aldo Faisal &copy; created 5.11.2000
 * NetTrader - visualisation, scientific and financial analysis and simulation system
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

#include "modigliani_base/sequential_statistics.h"

namespace modigliani_base {

/**
   @brief Create a Sequential_statistics
*/
Sequential_statistics::Sequential_statistics() {
  Reset();
}

Sequential_statistics::~Sequential_statistics() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 */
void Sequential_statistics::Reset() {
  counter_ = 0;
  average_ = 0.0;
  val_square_average_ = 0.0;
  variance_ = 0.0;
  min_ = 1 / 0.0;  // MAXFLOAT;//numeric_limits<Real>::max();
  max_ = -1 / 0.0;  // MINFLOAT;//numeric_limits<Real>::min();
}

/** @short
 @param      val value to add
 */
void Sequential_statistics::Add(Real val) {
  counter_++;

  average_ = UpdateAverage(average_, val, counter_);
  val_square_average_ = UpdateAverage(val_square_average_, val * val, counter_);
  variance_ = val_square_average_ - average_ * average_;

  if (val < min_)
    min_ = val;
  else if (val > max_)
    max_ = val;
}

/**
 *  @short
 *  \param  str ostream to output into
 *  \param  self
 *  \return ostream
 */
std::ostream & operator<<(std::ostream & str,
                          const Sequential_statistics & self) {
  str << self.counter() << "\t" << self.average() << "\t"
    << self.val_square_average() << "\t" << self.variance() << "\t"
    << self.min() << "\t" << self.max();
  return (str);
}

/** @short
 @param avg the current average
 @param val the value
 @param n size
 @return The new average
 @warning Does not update anything!
 */

Real Sequential_statistics::UpdateAverage(Real avg, Real val, Size n) const {
  return (val * (1.0 / n) + avg * (1.0 - 1.0 / n));
}

}  // namespace modigliani_base
