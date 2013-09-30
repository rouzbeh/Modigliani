/**@file histogram.h
 * Histogram class header
 * @author Ahmed Aldo Faisal &copy; created 11.10.2000
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_HISTOGRAM_H_
#define MODIGLIANI_MODIGLIANI_BASE_HISTOGRAM_H_

#include <vector>
#include <cmath>

#include "modigliani_base/main.h"
#include "modigliani_base/types.h"
#include "modigliani_base/obj.h"
#include "modigliani_base/error.h"

namespace modigliani_base {

  typedef std::vector < Size > HistogramBins;
/** @short Histogram class
 \bug unknown
 \warning unknown
 */
  class Histogram:public Obj {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Histogram(Real minVal = 0, Real maxVal = 1, Size numberOfBins = 10);
    Histogram(const Histogram & original);
    const Histogram & operator=(const Histogram & right);
     virtual ~Histogram();
    /* ***  Methods              ** */
    void Reset();
    void BinValue(Real value);
    void Show(bool showHeader = false) const;
     std::vector < Real > PDF() const;
    /* in bits */
    Real ShannonEntropy() const;

    friend std::ostream & operator<<(std::ostream & str,
                                     const Histogram & self);
    /* ***  Data                 ** */
    HistogramBins bins;
    Real numBins;

  protected:
    Real min;
    Real max;
    Real binWidth;
    Real numDataPoints;
    Size numOutliers;

  private:
  };
}  // namespace modigliani_base
#endif  // MODIGLIANI_MODIGLIANI_BASE_HISTOGRAM_H_
