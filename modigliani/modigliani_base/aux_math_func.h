/**
 * @file aux_math_func.h
 * Auxiliary functions header
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

#ifndef _modigliani_base_aux_math_func_h_
#define _modigliani_base_aux_math_func_h_

/* M core includes */
#include "types.h"
#include "obj.h"
/* M includes */
#include "mtrand.h"
/* other includes */
#include <cmath>

namespace modigliani_base {
/**
 * @short Mlimit01
 * Limit a value between [0,1]
 * \bug unknown
 * \warning unknown
 */
extern Real MComputePLogP(Real prob);
extern Real Mlimit01(Real value);
extern Real Mround(Real value);
extern Real MLogGammaFunction(Real arg);
extern MTRand MMakeMTRandFunction(MTRand::uint32 oneSeed);
extern int Misnan(double value);
extern int Misinf(double value);
}
/* *** DATA *** */
#ifndef M_PI
#define M_PI 3.14159265358979323846  /* pi */
#endif

#endif /* _modigliani_base_aux_math_func_h_ */

/* File skeleton generated by GenMObj version 0.6. */
