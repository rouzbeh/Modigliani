/**
 * @file error.cc
 * Implementation of classes (!) for error handling
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * NetTrader - finance management, analysis and simulation system
 * @version  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
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

#include "modigliani_base/error.h"

using modigliani_base::Error;
using modigliani_base::Dimension_error;
using modigliani_base::Bounds_error;
using modigliani_base::Precond_error;
using modigliani_base::Postcond_error;
using modigliani_base::Invariant_error;
using modigliani_base::Mem_error;


/**
   @brief check for bounds error

   @param f file name
   @param l line number
   @param i actual index
   @param lo low bound
   @param hi high bound
*/
void CheckBounds(const char *f, int l, int i, int lo, int hi) {
  if (lo <= i && i <= hi)
    return;
  M_THROW(Bounds_error(f, l, i, lo, hi));
}

/**
   @brief    check for dimension matching
   
   @param f file name
   @param l line number
   @param leftDim dimension of left expression to be equal
   @param rightDim dimension of right expression to be equal
*/
void CheckDimension(const char *f, int l, int leftDim, int rightDim) {
  if (leftDim != rightDim)
    M_THROW(Dimension_error(f, l, leftDim, rightDim));
}

/**
   @param f file name
   @param l line number
   @param i actual index
   @param lo low bound
   @param hi high bound
*/
Bounds_error::Bounds_error(const char *f, int l, int i, int lo, int hi)
:Error(f, l, 0, "Index bounds error"), _actual(i), _low(lo), _high(hi) {
}

/**
   @brief Report dimension error
   
   @param f file name
   @param l line number
   @param leftDim dimension of left expression to be equal
   @param rightDim dimension of right expression to be equal
*/
Dimension_error::Dimension_error(const char *f, int l, int left, int right)
:Error(f, l, 0, "Dimension mismatch error"), _left(left), _right(right) {
}

void Dimension_error::message(std::ostream & os) const {
  Error::message(os);
  os << "Dimension " << _left << " on the  left  mismatches dimension "
    << _right << " on the right.";
}

/**
   @brief report the error to a stream
    
   @param os output stream
*/
void Bounds_error::message(std::ostream & os) const {
  Error::message(os);
  os << "Index = " << _actual << " Range = " << _low << " ... " << _high;
}

/*-------------------------------------------------------------------------*/
/**
   @param f file name
   @param l line number
   @param expr expression generating failure
*/
Precond_error::Precond_error(const char *f, int l, const char *expr)
:Error(f, l, expr, "Precondition error") {
}

/**
   @param f file name
   @param l line number
   @param expr expression generating failure
*/
Postcond_error::Postcond_error(const char *f, int l, const char *expr)
    :Error(f, l, expr, "Postcondition error") {
}

/**
   @param f file name
   @param l line number
   @param expr expression generating failure
*/
Invariant_error::Invariant_error(const char *f, int l, const char *expr)
    :Error(f, l, expr, "Invariant error") {
}

/**
   @param f file name
   @param l line number
   @param expr expression generating failure
*/
Mem_error::Mem_error(const char *f, int l, const char *expr)
    :Error(f, l, expr, "Memory allocation error") {
}

/**
   @param f file name
   @param l line number
   @param expr expression generating failure
   @param m error message
*/
Error::Error(const char *f, int l, const char *e, const char *m)
    :_file(f), _line(l), _expr(e), _msg(m) {
}

/**
   @brief report the error to a stream
    
   @param os output stream
*/
void Error::message(std::ostream & os) const {
  if (_msg)
    os << _msg;
  else if (_expr)
    os << "Assertion failure";
  os << "\nFile " << _file << " line " << _line << " ";
  if (_expr)
    os << _expr;
}

/**
   @brief error and abort program
*/
void Error::abort() {
  message(std::cerr);
  std::cerr << std::endl;
  // set a breakpoint here and watch the stack
  exit(1);
}
