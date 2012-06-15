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

#include "error.h"

using namespace modigliani_base;

void checkBounds(const char* f, int l, int i, int lo, int hi)
/* PURPOSE:    check for bounds error
   RECEIVES:   f - file name
           l - line number
           i - actual index
           lo, hi - low, high bounds
*/
{
    if (lo <= i && i <= hi) return;
    M_THROW(Bounds_error(f, l, i, lo, hi));
}

/* PURPOSE:    check for dimension matching
   RECEIVES:   f - file name
           l - line number
           leftDim, rightDim dimension of left and right expression to be equal
*/
void checkDimension(const char* f, int l, int leftDim, int rightDim)
{
    if (leftDim != rightDim)
        M_THROW(Dimension_error(f, l, leftDim, rightDim));
}
/* OPERATIONS **************************************************************/

Bounds_error::Bounds_error(const char* f, int l, int i, int lo, int hi)
/* RECEIVES:   f - file name
               l - line number
               i - actual index
               lo, hi - low, high bounds
*/
        : Error(f, l, 0, "Index bounds error"),
        _actual(i),
        _low(lo),
        _high(hi)
{}
/*-------------------------------------------------------------------------*/

Dimension_error::Dimension_error(const char* f, int l, int left, int right)
        :
        Error(f, l, 0, "Dimension mismatch error"),
        _left(left),
        _right(right)
{

}

void Dimension_error::message(std::ostream& os) const
{
    Error::message(os);
    os << "Dimension "<< _left <<" on the  left  mismatches dimension "
    << _right<<" on the right.";
}
/*.........................................................................*/

void Bounds_error::message(std::ostream& os) const
/* PURPOSE:    report the error to a stream
   RECEIVES:   os - output stream
*/
{
    Error::message(os);
    os << "Index = " << _actual << " Range = " << _low << " ... "
    << _high ;
}

/*-------------------------------------------------------------------------*/

Precond_error::Precond_error(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : Error(f, l, expr, "Precondition error")
{}

/*-------------------------------------------------------------------------*/

Postcond_error::Postcond_error(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : Error(f, l, expr, "Postcondition error")
{}

/*-------------------------------------------------------------------------*/

Invariant_error::Invariant_error(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : Error(f, l, expr, "Invariant error")
{}

/*-------------------------------------------------------------------------*/

Mem_error::Mem_error(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : Error(f, l, expr, "Memory allocation error")
{}

/*-------------------------------------------------------------------------*/

Error::Error(const char* f, int l, const char* e, const char* m)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
               m - error message
*/
        :
        _file(f),
        _line(l),
        _expr(e),
        _msg(m)
{}

/*.........................................................................*/

void Error::message(std::ostream& os) const
/* PURPOSE:    report the error to a stream
   RECEIVES:   os - output stream
*/
{
    if (_msg) os << _msg;
    else if (_expr) os << "Assertion failure";
    os << "\nFile " << _file << " line " << _line << " ";
    if (_expr) os << _expr;
}
/*.........................................................................*/

void Error::abort()
/* PURPOSE:    report error and abort program
*/
{
    message(std::cerr);
    std::cerr << std::endl;
    // set a breakpoint here and watch the stack
    exit(1);
}

