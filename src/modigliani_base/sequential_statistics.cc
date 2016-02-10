/**
 * @file sequential_statistics.cc
 * @brief Sequential_statistics class implementation
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

#include "modigliani_base/sequential_statistics.h"

namespace modigliani_base {
Sequential_statistics::Sequential_statistics() {
  Reset();
}

Sequential_statistics::~Sequential_statistics() {
}

void Sequential_statistics::Reset() {
  counter_ = 0;
  average_ = 0.0;
  val_square_average_ = 0.0;
  variance_ = 0.0;
  min_ = 1 / 0.0;  // MAXFLOAT;//numeric_limits<Real>::max();
  max_ = -1 / 0.0;  // MINFLOAT;//numeric_limits<Real>::min();
}

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

std::ostream & operator<<(std::ostream & str,
                          const Sequential_statistics & self) {
  str << self.counter() << "\t" << self.average() << "\t"
      << self.val_square_average() << "\t" << self.variance() << "\t"
      << self.min() << "\t" << self.max();
  return (str);
}

Real Sequential_statistics::UpdateAverage(Real avg, Real val, Size n) const {
  return (val * (1.0 / n) + avg * (1.0 - 1.0 / n));
}
}  // namespace modigliani_base
