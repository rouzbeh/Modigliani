/**
 * @file Rnd_dist.h
 * Header for random distribution super class
 * @author Ahmed A. Faisal, 15. 10. 1998(c)
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

#ifndef _mbase_rnd_dist_h_
#define _mbase_rnd_dist_h_

#include "types.h"
#include "Obj.h"
#include "Vector.h"

#include "MTrand.h"

namespace modigliani_base{
class Rnd_dist : public Obj {
public:
    Rnd_dist();
    Rnd_dist(int dim);
    Rnd_dist(const Rnd_dist &);
    virtual ~Rnd_dist();

    virtual Real RndVal() const = 0;
    //  Real Rnd_dist();
    // M_vector_o Rnd_dist();

    Real _mean() const   {
        return (mean);
    };
    Real _variance() const   {
        return (variance);
    };

    friend std::ostream & operator<<(std::ostream & os, const Rnd_dist & self);

protected:
    float uniformQuickDirty() const;
    float uniform1() const;
    float uniformMT() const {
        return (randgen.rand());
    }
    // 2DO: seed has to be mutexed !
    Real mean;
    Real variance;
private:
    static MTRand randgen;
    static long seed;
};
}

#endif /* _mbase_rnd_dist_h_ */




