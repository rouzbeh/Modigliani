/* nt_error_obj.h - general classes (!) for error handling  */
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


/* $Id: nt_error_obj.h,v 1.3 2003/06/20 16:15:09 face Exp $
 * $Log: nt_error_obj.h,v $
 * Revision 1.3  2003/06/20 16:15:09  face
 * *** empty log message ***
 *
 * Revision 1.2  2003/06/20 13:26:00  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/05/11 11:32:46  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.2  1999/07/03 16:26:19  face
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
 * Revision 2.2  1999/06/06 23:51:28  face
 * *** empty log message ***
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.5  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.5  1998/12/25 21:28:17  face
 *  new NetTrader version 0.2 set with this revision of this file
 * n
 *
 * Revision 1.4  1998/10/20 14:51:52  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/10/19 14:09:03  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/09/28 00:15:27  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/06/02 20:07:34  face
 * Initial revision
 *
 *
 */


#ifndef _NT_ERROR_OBJ_H_
#define _NT_ERROR_OBJ_H_


#include <iostream>
#include <cstdlib>


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

/** NT superclass to report an error */
class NT_error_o : public NT_o
{
public:
    NT_error_o(const char* f, int l, const char* expr = 0, const char* msg = 0);
    void abort();
    virtual void message(ostream&) const;

private:
    const char* _file;
    int _line;
    const char* _expr;
    const char* _msg;
};

/*-------------------------------------------------------------------------*/

/** Class to report an integer bound error
*/
class NT_bounds_error_o : public NT_error_o
{
public:
    NT_bounds_error_o(const char* f, int l, int i, int lo, int hi);
    virtual void message(ostream&) const;
private:
    int _actual;
    int _low;
    int _high;
};

/** Class to report an dimension mismatch error */
class NT_dimension_error_o : public NT_error_o
{
public:
    NT_dimension_error_o(const char* f, int l, int left, int right);
    virtual void message(ostream&) const;
private:
    int _left;
    int _right;
};

/*-------------------------------------------------------------------------*/

class NT_precond_error_o : public NT_error_o
            /** Class to report a precondition failure */
{
public:
    NT_precond_error_o(const char* f, int l, const char* expr);
};

/*-------------------------------------------------------------------------*/

class NT_postcond_error_o : public NT_error_o
            /** Class to report a postondition failure
            */
{
public:
    NT_postcond_error_o(const char* f, int l, const char* expr);
};

/*-------------------------------------------------------------------------*/

class NT_invariant_error_o : public NT_error_o
            /** Class to report an invariant failure
            */

{
public:
    NT_invariant_error_o(const char* f, int l, const char* expr);
};

/*-------------------------------------------------------------------------*/

class NT_mem_error_o : public NT_error_o
            /** Class to report memory exhaustion
            */
{
public:
    NT_mem_error_o(const char* f, int l, const char* expr);
};



#ifdef NT_THROW_ERRORS
#define NT_THROW(E) (throw E)
#else
#define NT_THROW(E) (E.abort())
#endif

#define NT_ASSERT(E) ((E) ? (void)0 : NT_THROW(NT_error_o(__FILE__, __LINE__, #E)))
#define NT_ASSERT_PRECOND(E) ((E) ? (void)0 : NT_THROW(NT_precond_error_o(__FILE__, __LINE__, #E)))
#define NT_ASSERT_POSTCOND(E) ((E) ? (void)0 : NT_THROW(NT_postcond_error_o(__FILE__, __LINE__, #E)))
#define NT_ASSERT_INVARIANT(E) ((E) ? (void)0 : NT_THROW(NT_invariant_error_o(__FILE__, __LINE__, #E)))
#define NT_ASSERT_MEM(E) ((E) ? (void)0 : NT_THROW(NT_mem_error_o(__FILE__, __LINE__, #E)))
#define NT_ASSERT_BOUNDS(I, L, H) (NT_CheckBounds(__FILE__, __LINE__, I, L, H))
#define NT_ASSERT_DIM(L, R) (NT_CheckDimension(__FILE__, __LINE__, L, R))

extern void NT_CheckBounds(const char* f, int l, int i, int lo, int hi);
extern void NT_CheckDimension(const char* f, int l, int leftDim, int rightDim);

#endif /* _NT_ERROR_OBJ_H_ */
