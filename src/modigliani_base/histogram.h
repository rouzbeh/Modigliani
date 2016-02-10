/**@file histogram.h
 * @brief Histogram class header
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_HISTOGRAM_H_
#define MODIGLIANI_MODIGLIANI_BASE_HISTOGRAM_H_

#include <vector>
#include <cmath>

#include "modigliani_base/main.h"
#include "modigliani_base/types.h"
#include "modigliani_base/obj.h"
#include "modigliani_base/error.h"

namespace modigliani_base {
  /**
   * @brief Histogram bins, implemented as a vector.
   */
  typedef std::vector < Size > HistogramBins;
  
  /**
   * @class Histogram
   * @brief Histogram class
   */
  class Histogram:public Obj {
 public:
    /**
     * @brief Creates a Histogram
     * @param minValue Minimum value, default 0
     * @param maxValue Maximum value, default 1
     * @param numberOfBins Number of bins, default 10
     */
    Histogram(Real minVal = 0, Real maxVal = 1, Size numberOfBins = 10);
    
    Histogram(const Histogram & original) = delete;
    
    const Histogram & operator=(const Histogram & right) = delete;

    /**
     * @brief Destructor
     */
    virtual ~Histogram();
    
    /**
     * @brief Reset the histogram class to object creation values
     */
    void Reset();
    
    /**
     * @brief Insert the value in the appropriate bin.
     * if the value is outside the range the numOutliers
     * counter variable will be increased instead.
     @param      value The value to insert
     @warning    Bound checking is implicitly done by increasing numOutliers
    */
    void BinValue(Real value);
    
    /**
     * @brief Prints the histogram to stdout
     * @param showHeader Whether to show the header, default false
     */
    void Show(bool showHeader = false) const;

    /**
     * @brief Returns the probability density functions.
     * @return vector containing the PDF
     */
    std::vector < Real > PDF() const;
    
    /**
     * @brief Return Shannon entropy
     * @return Entropy in bits
     */
    Real ShannonEntropy() const;

    /**
     * @brief Prints the histogram
     * @param  str ostream to output into
     * @param  self
     * @return ostream
     */
    friend std::ostream & operator<<(std::ostream & str,
                                     const Histogram & self);

    /**
     * brief Histogram bins
     */
    HistogramBins bins;

    /**
     * @brief Number of bins
     */
    Real numBins;
    
 protected:
    Real min;
    Real max;
    Real binWidth;
    Real numDataPoints;
    Size numOutliers;
  };
}  // namespace modigliani_base
#endif  // MODIGLIANI_MODIGLIANI_BASE_HISTOGRAM_H_
