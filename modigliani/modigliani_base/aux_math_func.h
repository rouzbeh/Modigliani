/**
 * @file aux_math_func.h
 * Auxiliary math functions header
 * @author Ahmed Aldo Faisal &copy; created 27.4.2000
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_AUX_MATH_FUNC_H_
#define MODIGLIANI_MODIGLIANI_BASE_AUX_MATH_FUNC_H_

#include <cmath>

#include "modigliani_base/types.h"
#include "modigliani_base/obj.h"

namespace modigliani_base {

extern Real ComputePLogP(Real prob);
/**
 * @short Mlimit01
 * Limit a value between [0,1]
 */
extern Real Limit01(Real value);
extern Real Round(Real value);
extern Real LogGammaFunction(Real arg);
extern int IsNAN(const Real value);
extern int IsInf(const Real value);

#ifndef PI
#define PI 3.14159265358979323846
#endif
}  // namespace modigliani_base

#endif  // MODIGLIANI_MODIGLIANI_BASE_AUX_MATH_FUNC_H_
