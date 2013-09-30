/**
 * @file error.h
 * General classes (!) for error handling
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * NetTrader - finance management, analysis and simulation system
 * @version  0.2
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_ERROR_H_
#define MODIGLIANI_MODIGLIANI_BASE_ERROR_H_

#include <cstdlib>

#include "modigliani_base/types.h"
#include "modigliani_base/obj.h"

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif
#define debug_print(fmt, ...) \
        do { if (DEBUG_TEST) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

namespace modigliani_base {
/**
 *  @brief superclass to report an error
 */
  class Error:public Obj {
  public:
    Error(const char *f, int l, const char *expr = 0, const char *msg = 0);
    void abort();
    virtual void message(std::ostream &) const;

  private:
    const char *_file;
    int _line;
    const char *_expr;
    const char *_msg;
  };

/** @brief Class to report an integer bound error
 */
  class Bounds_error:public Error {
  public:
    Bounds_error(const char *f, int l, int i, int lo, int hi);
    virtual void message(std::ostream &) const;
  private:
    int _actual;
    int _low;
    int _high;
  };

/** @brief Class to report an dimension mismatch error */
  class Dimension_error:public Error {
  public:
    Dimension_error(const char *f, int l, int left, int right);
    virtual void message(std::ostream &) const;
  private:
    int _left;
    int _right;
  };

/** @brief Class to report a precondition failure */
  class Precond_error:public Error  {
  public:
    Precond_error(const char *f, int l, const char *expr);
  };

/** @brief Class to report a postondition failure
 */
  class Postcond_error:public Error  {
  public:
    Postcond_error(const char *f, int l, const char *expr);
  };

/** Class to report an invariant failure
 */
  class Invariant_error:public Error  {
  public:
    Invariant_error(const char *f, int l, const char *expr);
  };

/** Class to report memory exhaustion
 */
  class Mem_error:public Error  {
  public:
    Mem_error(const char *f, int l, const char *expr);
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

  extern void CheckBounds(const char *f, int l, int i, int lo, int hi);
  extern void CheckDimension(const char *f, int l, int leftDim, int rightDim);
}  // namespace modigliani_base
#endif  // MODIGLIANI_MODIGLIANI_BASE_ERROR_H_
