/**\file Uniform_vec_rnd_dist.h - Uniform_vec_rnd_dist class header
 * by Ahmed Aldo Faisal &copy; created 26.6.1999
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.3
 * Copyright (C) 1998,199 Ahmed Aldo Faisal
 *
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

#ifndef _mbase_uniform_vec_rnd_dist_h_
#define _mbase_uniform_vec_rnd_dist_h_


#include "types.h"
#include "Obj.h"

#include "Vec_rnd_dist.h"
#include "Uniform_rnd_dist.h"

/** @short Uniform_vec_rnd_dist class

\bug unknown
\warning unknown
 */
namespace mbase{
class Uniform_vec_rnd_dist : public Vec_rnd_dist {
public:
    /***   Constructors, CopyAssignment and Destructor  ***/
    Uniform_vec_rnd_dist(Real lowerBound, Real upperBound, Msize dim);
    Uniform_vec_rnd_dist(Vector lowerBound, Vector upperBound, Msize dim);

    Uniform_vec_rnd_dist(const Uniform_vec_rnd_dist & original);
    const Uniform_vec_rnd_dist & operator= (const Uniform_vec_rnd_dist & right);
    virtual ~Uniform_vec_rnd_dist();
    /* ***  Methods              ***/
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    Vector lowerBoundVec;
    Vector upperBoundVec;
};
}
#endif /* _mbase_uniform_vec_rnd_dist_h_ */

/* File skeleton generated by GenMObj version 0.4. */
