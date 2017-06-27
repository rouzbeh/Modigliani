/**
 * @file types.h
 * @brief Type declarations for Modigliani
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * Copyright (C) 2013 Ali Neishabouri
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_TYPES_H_
#define MODIGLIANI_MODIGLIANI_BASE_TYPES_H_

#include <string>
#include <cstdlib>
#include <cmath>
#include <iostream>

namespace modigliani {
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
    if (currentTalkLevel >= talkLevel)
      std::cerr << message << std::endl;
  }

  static const double pi_ = 3.14159265358979323846;
  static const double e_ = 2.7182818284590452354;
  static const double sqrt_2_ = 1.41421356237309504880;

  template < class T > inline T M_Max(T a, T b) {
    return (a > b ? a : b);
  }
  
  template < class T > inline T M_Min(T a, T b) {
    return (a < b ? a : b);
  }
  
  template < class T > inline T M_Abs(T x) {
    return ((x) >= 0 ? (x) : -(x));
  }
  
  template < class T > inline void M_Swap(T & a, T & b) {
    T temp = a;
    a = b;
    b = temp;
  }
}  // namespace modigliani

#endif  // MODIGLIANI_MODIGLIANI_BASE_TYPES_H_
