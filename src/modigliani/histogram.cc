/**@file histogram.cc
 * @brief Histogram class implementation
 * 
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

#include "modigliani/histogram.h"

namespace modigliani {
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

Histogram::~Histogram() {
}

void Histogram::Reset() {
  bins.clear();
  numDataPoints = 0;
  numOutliers = 0;
}

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
}  // namespace modigliani

