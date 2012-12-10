/**
 * @file types.h
 * Type declarations for modigliani
 * @author Ali Neishabouri
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * @version  1
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_TYPES_H_
#define MODIGLIANI_MODIGLIANI_BASE_TYPES_H_

#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>

namespace modigliani_base {
typedef double Real;

typedef unsigned int Size;
typedef long int Long;
typedef Long Mnum;

typedef long int Mid;

enum ReturnEnum {
  SUCCESS,
  FAIL,
  NOT_IMPLEMENTED,
  NOT_DERIVED,
  OUT_OF_MEMORY,
  FILE_NOT_FOUND,
  WRONG_FILE_TYPE,
  BAD_FILE,
  EMPTY_FILE,
  BAD_DATA,
  READ_FAIL,
  NO_SPACE_LEFT,
  WRITE_FAIL,
  NOT_EXISTING_NODE,
  NOT_EXISTING_EDGE,
  PARAM_OUT_OF_RANGE,
  PARAM_UNSUPPORTED
};

inline void Cerr(Size currentTalkLevel, Size talkLevel, std::string message) {
  if (currentTalkLevel >= talkLevel) std::cerr << message << std::endl;
}

#define M_CERR( L , M )  (M_cerr( M_TALKLEVEL, L , M ))

static const double pi_ = 3.14159265358979323846;
static const double e_ = 2.7182818284590452354;
static const double sqrt_2_ = 1.41421356237309504880;

#define M_SELF (*this)

template<class T> inline T M_Max(T a, T b) {
  return (a > b ? a : b);
}
template<class T> inline T M_Min(T a, T b) {
  return (a < b ? a : b);
}
template<class T> inline T M_Abs(T x) {
  return ((x) >= 0 ? (x) : -(x));
}
template<class T> inline void M_Swap(T& a, T& b) {
  T temp = a;
  a = b;
  b = temp;
}
}

#endif  // MODIGLIANI_MODIGLIANI_BASE_TYPES_H_
