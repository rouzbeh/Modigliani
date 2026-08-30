/**
 * @file aux_math_func.h
 * @brief Auxiliary math functions header
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_AUX_MATH_FUNC_H_
#define MODIGLIANI_MODIGLIANI_BASE_AUX_MATH_FUNC_H_

#include <cmath>

#include "modigliani/types.h"
#include "modigliani/obj.h"

namespace modigliani {
  /**
   * @brief Computes @f$p\times log(p)@f$
   * @param prob p value
   * @return @f$p\times log(p)@f$
   */
  extern Real ComputePLogP(Real prob);
  
  /**
   * @brief Limit a value between [0,1]
   */
  extern Real Limit01(Real value);

  /**
   * @brief Rounds a value to the nearest integer.
   * @param value Value to round
   * @return Rounded value
   */   
   Real Round(Real value);

  /**
   * @brief Fix the base value from which all RNG seeds are derived.
   *
   * Call this before constructing any model object to make a run
   * reproducible. Without it the base is drawn from std::random_device.
   * @param base The base seed
   */
  extern void SetSeedBase(unsigned int base);

  /**
   * @brief The base seed in use, drawing one if that has not happened yet.
   *
   * Log this and pass it back via SetSeedBase() to repeat a run.
   * @return The base seed
   */
  extern unsigned int SeedBase();

  /**
   * @brief Return the next distinct seed for a random number generator.
   *
   * Every generator in the program - one per ion channel population, per
   * compartment, per synaptic background process - must get its own seed.
   * Successive calls return well separated values derived from SeedBase().
   * @return A seed
   */
  extern unsigned int NextSeed();

  /**
   * @brief Compute the logarithm of the Gamma function.
   *
   * Taken from function 'gammln' Numerical Recipes in C (Press et
   * al.)
   * @param arg Function argument
   * @return Result
   */
  extern Real LogGammaFunction(Real arg);

  /**
   * @brief Checks for NaN
   * @param value The value to check
   * @return 0 if NaN, rounded value otherwise
   */
  extern int IsNAN(const Real value);

  /**
   * @brief Checks infinity
   * @param value The value to check
   * @return 1 for +infinity, -1 for -infinity, 0 otherwise
   */
  extern int IsInf(const Real value);
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_BASE_AUX_MATH_FUNC_H_
