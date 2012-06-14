/**
 *  @file Error.h - general classes (!) for error handling
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

#ifndef _modigliani_base_error_h_
#define _modigliani_base_error_h_

#include <iostream>
#include <cstdlib>

#include "types.h"
#include "Obj.h"


namespace modigliani_base {
/**
 *  superclass to report an error
 *
 */

class Error: public Obj {
public:
	Error(const char* f, int l, const char* expr = 0, const char* msg = 0);
	void abort();
	virtual void message(std::ostream&) const;

private:
	const char* _file;
	int _line;
	const char* _expr;
	const char* _msg;
};

/*-------------------------------------------------------------------------*/

/** Class to report an integer bound error
 */
class Bounds_error: public Error {
public:
	Bounds_error(const char* f, int l, int i, int lo, int hi);
	virtual void message(std::ostream&) const;
private:
	int _actual;
	int _low;
	int _high;
};

/** Class to report an dimension mismatch error */
class Dimension_error: public Error {
public:
	Dimension_error(const char* f, int l, int left, int right);
	virtual void message(std::ostream&) const;
private:
	int _left;
	int _right;
};

/*-------------------------------------------------------------------------*/

class Precond_error: public Error
/** Class to report a precondition failure */
{
public:
	Precond_error(const char* f, int l, const char* expr);
};

/*-------------------------------------------------------------------------*/

class Postcond_error: public Error
/** Class to report a postondition failure
 */
{
public:
	Postcond_error(const char* f, int l, const char* expr);
};

/*-------------------------------------------------------------------------*/

class Invariant_error: public Error
/** Class to report an invariant failure
 */

{
public:
	Invariant_error(const char* f, int l, const char* expr);
};

/*-------------------------------------------------------------------------*/

class Mem_error: public Error
/** Class to report memory exhaustion
 */
{
public:
	Mem_error(const char* f, int l, const char* expr);
};

#ifdef M_THROW_ERRORS
#define M_THROW(E) (throw E)
#else
#define M_THROW(E) (E.abort())
#endif

#define M_ASSERT(E) ((E) ? (void)0 : M_THROW(modigliani_base::Error(__FILE__, __LINE__, #E)))
#define M_ASSERT_PRECOND(E) ((E) ? (void)0 : M_THROW(modigliani_base::Precond_error(__FILE__, __LINE__, #E)))
#define M_ASSERT_POSTCOND(E) ((E) ? (void)0 : M_THROW(modigliani_base::Postcond_error(__FILE__, __LINE__, #E)))
#define M_ASSERT_INVARIAM(E) ((E) ? (void)0 : M_THROW(modigliani_base::Invariant_error(__FILE__, __LINE__, #E)))
#define M_ASSERT_MEM(E) ((E) ? (void)0 : M_THROW(modigliani_base::Mem_error(__FILE__, __LINE__, #E)))
#define M_ASSERT_BOUNDS(I, L, H) (checkBounds(__FILE__, __LINE__, I, L, H))
#define M_ASSERT_DIM(L, R) (checkDimension(__FILE__, __LINE__, L, R))

extern void checkBounds(const char* f, int l, int i, int lo, int hi);
extern void checkDimension(const char* f, int l, int leftDim, int rightDim);
}
#endif /* _M_ERROR_OBJ_H_ */
