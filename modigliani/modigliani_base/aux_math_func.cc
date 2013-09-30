/**
 * @file aux_math_func.cc
 * Auxiliary functions implementation
 * @author Ahmed Aldo Faisal &copy; created 27.4.2000
 * NetTrader - visualisation, scientific and financial analysis and simulation system
 * @version  0.4
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

#include "modigliani_base/aux_math_func.h"

using modigliani_base::Real;

Real modigliani_base::ComputePLogP(Real prob) {
    if (0.0 == prob)
        return (0);
    else if (prob < 0.0) {
        std::cerr
            << "ComputeEntropyElement - Error :"
            << " Negative probability encountered."
                << std::endl;
        return (0);
    } else {
        return (prob * log(prob) / 0.69314718055994530942 /* log(2) */);
    }
}

Real modigliani_base::Limit01(Real value) {
    if (value > 1.0)
        return (1.0);
    else if (value < 0.0)
        return (0.0);
    else
        return (value);
}

Real modigliani_base::Round(Real value) {
    return ((Real) (floor(value + 0.5)));
}

/**    Compute the logarithm of the Gamma function
 Taken from function 'gammln' Numerical Recipes in C (Press et al.)
 */
Real modigliani_base::LogGammaFunction(Real arg) {
    double x, tmp, ser;
    static double cof[6] = { 76.18009173, -86.50532033, 24.01409822,
            -1.231739516, 0.120858003e-2, -0.536382e-5 };
    int j;

    x = arg - 1.0;
    tmp = x + 5.5;
    tmp -= (x + 0.5) * log(tmp);
    ser = 1.0;
    for (j = 0; j <= 5; j++) {
        x += 1.0;
        ser += cof[j] / x;
    }
    return (-tmp + log(2.50662827465 * ser));
}

int modigliani_base::IsNAN(const Real value) {
  return (static_cast<int>(value != value));
}

int modigliani_base::IsInf(const Real x) {
    double y = x - x;
    int s = (y != y);

    if (s && x > 0)
        return (+1);
    else if (s && x < 0)
        return (-1);
    else
        return (0);
}
