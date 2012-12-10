/**
 * @file binomial_rnd_dist.h
 * Binomial_rnd_dist class header
 * @author Ahmed Aldo Faisal &copy; created 19.6.2001
 * @version  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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

#ifndef _modigliani_base_binomial_rnd_dist_h_
#define _modigliani_base_binomial_rnd_dist_h_

/* M core includes */
#include "types.h"
#include "obj.h"
/* Parent includes */
#include "rnd_dist.h"
/* M includes */

/* other includes */
#include <cmath>
#include <vector>

namespace modigliani_base {
/** @short Binomial_rnd_dist class
 \bug unknown
 \warning unknown
 */
class Binomial_rnd_dist: public Rnd_dist {

    class Initializer {
    public:
        Initializer();
        std::vector<Real> logGammaTable;
    };

    friend class Initializer;
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Binomial_rnd_dist(Real pp, Size n);
    Binomial_rnd_dist(const Binomial_rnd_dist & original);
    const Binomial_rnd_dist & operator=(const Binomial_rnd_dist & right);
    virtual ~Binomial_rnd_dist();
    Real RndVal() const;
    /** Identical to method 'bnldev' from Numerical Recipes in C (Press et al.) */
    Real Binomial(float pp, int n) const;
    Real LogGamma(Real val) const;
    /* ***  Methods              ***/
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    //static vector<Real> logGammaTable;
    static bool init;
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    Real p;
    Size n;
    static Initializer static_table;
};
}
#endif /* _modigliani_base_binomial_rnd_dist_h_ */

/* File skeleton generated by GenMObj version 0.7. */
