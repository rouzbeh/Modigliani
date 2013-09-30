/**\file histogram.cc - Histogram class implementation
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

#include "modigliani_base/histogram.h"

namespace modigliani_base {

/** @brief Creates a Histogram
 */
Histogram::Histogram(Real minVal,
                     Real maxVal,
                     Size numberOfBins):bins(static_cast<int>(numberOfBins)) {
  min = minVal;
  max = maxVal;
  numBins = numberOfBins;
  binWidth = (fabs(max - min)) / numBins;
  numDataPoints = 0;
  numOutliers = 0;
  M_ASSERT(min < max);
  M_ASSERT(numberOfBins > 0);
}

/* ***      COPY AND ASSIGNMEM    ***/
Histogram::Histogram(const Histogram & original):Obj() {
  bins = original.bins;
  min = original.min;
  max = original.max;
  numBins = original.numBins;
  binWidth = original.binWidth;
  numDataPoints = original.numDataPoints;
  numOutliers = original.numOutliers;
}

const Histogram & Histogram::operator=(const Histogram & right) {
  if (this == &right)
    return (*this);             // Gracefully handle self assignment
  bins = right.bins;
  min = right.min;
  max = right.max;
  numBins = right.numBins;
  binWidth = right.binWidth;
  numDataPoints = right.numDataPoints;
  numOutliers = right.numOutliers;
  return (*this);
}

/* ***      DESTRUCTOR        ***/
Histogram::~Histogram() {
}

/* ***  PUBLIC                                    ***   */
/** @short    Reset the histogram class to object creation values
 */
void Histogram::Reset() {
  bins.clear();
  numDataPoints = 0;
  numOutliers = 0;
}

/** @short      Insert the value in the appropriate bin,
 if the value is outside the range the numOutliers
 counter variable will be increased instead.
 @param      value
 @return     none
 \warning    Bound checking is implicitly done by increasing numOutliers
 \bug        unknown
 */
void Histogram::BinValue(Real value) {
  numDataPoints++;
  if ((value > max) || (value < min))
    numOutliers++;
  else
    (bins[floor(0.5 + (value - min) / (Real) binWidth)]) += 1;
}

std::vector < Real > Histogram::PDF() const {
  std::vector < Real > probVec(numBins);
  for (Size ll = 0; ll < numBins; ll++) {
    probVec[ll] = ((Real) bins[ll]) / ((Real) numDataPoints);
  }
  return (probVec);
}

Real Histogram::ShannonEntropy() const {
  Real h = 0;
  Real p = 0;
  for (Size ll = 0; ll < numBins; ll++) {
    p = ((Real) bins[ll]) / ((Real) numDataPoints);
    if (0.0 != p)
      h -= p * log(p);
  }
  return (h / log(2.0));
}

/**
   @brief
   @param showHeader Whether to show the header
 */
void Histogram::Show(bool showHeader) const {
  if (showHeader == true)
    std::cout << "Histogram " << numBins << " bins [" << min << "," << max
      << "] with " << numDataPoints << " of which " << numOutliers
      << " outliers." << std::endl;
  for (Size ll = 0; ll < numBins; ll++) {
    std::cout << bins[ll] << " ";
  }
  std::cout << std::endl;
}

std::ostream & operator<<(std::ostream & str, const Histogram & self) {
  for (Size ll = 0; ll < self.numBins; ll++) {
    str << self.bins[ll] << "\n ";
  }
  return (str);
}
}  // namespace modigliani_base

