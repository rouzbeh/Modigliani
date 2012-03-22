/* nt_error_obj.cc - implementation of classes (!) for error handling  */
/* by Ahmed A. Faisal, 22. 5. 1998(c) */


/*
 * NetTrader - finance management, analysis and simulation system
 * Version:  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/* $Id: nt_error_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_error_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/05/11 11:32:46  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.2  1999/07/03 16:26:18  face
 * edited and added new nt_error_o class:
 * nt_dimension_error_o
 * to report dimension mismatches between (e.g.NT_VECTOR_O)
 * bjects
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.1  1998/06/02 20:08:01  face
 * Initial revision
 *
 *
 */

#include "nt_error_obj.h"

void NT_CheckBounds(const char* f, int l, int i, int lo, int hi)
/* PURPOSE:    check for bounds error
   RECEIVES:   f - file name
	       l - line number
	       i - actual index
	       lo, hi - low, high bounds
*/
{
    if (lo <= i && i <= hi) return;
    NT_THROW(NT_bounds_error_o(f, l, i, lo, hi));
}

/* PURPOSE:    check for dimension matching
   RECEIVES:   f - file name
	       l - line number
	       leftDim, rightDim dimension of left and right expression to be equal
*/
void NT_CheckDimension(const char* f, int l, int leftDim, int rightDim)
{
    if (leftDim != rightDim)
        NT_THROW(NT_dimension_error_o(f, l, leftDim, rightDim));
}
/* OPERATIONS **************************************************************/

NT_bounds_error_o::NT_bounds_error_o(const char* f, int l, int i, int lo, int hi)
/* RECEIVES:   f - file name
               l - line number
               i - actual index
               lo, hi - low, high bounds
*/
        : NT_error_o(f, l, 0, "Index bounds error"),
        _actual(i),
        _low(lo),
        _high(hi)
{}
/*-------------------------------------------------------------------------*/

NT_dimension_error_o::NT_dimension_error_o(const char* f, int l, int left, int right)
        :
        NT_error_o(f, l, 0, "Dimension mismatch error"),
        _left(left),
        _right(right)
{

}

void NT_dimension_error_o::message(ostream& os) const
{
    NT_error_o::message(os);
    os << "Dimension "<< _left <<" on the  left  mismatches dimension "
    << _right<<" on the right.";
}
/*.........................................................................*/

void NT_bounds_error_o::message(ostream& os) const
/* PURPOSE:    report the error to a stream
   RECEIVES:   os - output stream
*/
{
    NT_error_o::message(os);
    os << "Index = " << _actual << " Range = " << _low << " ... "
    << _high ;
}

/*-------------------------------------------------------------------------*/

NT_precond_error_o::NT_precond_error_o(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : NT_error_o(f, l, expr, "Precondition error")
{}

/*-------------------------------------------------------------------------*/

NT_postcond_error_o::NT_postcond_error_o(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : NT_error_o(f, l, expr, "Postcondition error")
{}

/*-------------------------------------------------------------------------*/

NT_invariant_error_o::NT_invariant_error_o(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : NT_error_o(f, l, expr, "Invariant error")
{}

/*-------------------------------------------------------------------------*/

NT_mem_error_o::NT_mem_error_o(const char* f, int l, const char* expr)
/* RECEIVES:   f - file name
               l - line number
               expr - expression generating failure
*/
        : NT_error_o(f, l, expr, "Memory allocation error")
{}

/*-------------------------------------------------------------------------*/

NT_error_o::NT_error_o(const char* f, int l, const char* e, const char* m)
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

void NT_error_o::message(ostream& os) const
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

void NT_error_o::abort()
/* PURPOSE:    report error and abort program
*/
{
    message(cerr);
    cerr << endl;
    // set a breakpoint here and watch the stack
    exit(1);
}

