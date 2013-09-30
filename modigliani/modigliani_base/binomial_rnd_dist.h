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

#ifndef MODIGLIANI_MODIGLIANI_BASE_BINOMIAL_RND_DIST_H_
#define MODIGLIANI_MODIGLIANI_BASE_BINOMIAL_RND_DIST_H_


#include <cmath>
#include <vector>

#include "modigliani_base/types.h"
#include "modigliani_base/obj.h"
#include "modigliani_base/error.h"
#include "modigliani_base/rnd_dist.h"

namespace modigliani_base {
/** @short Binomial_rnd_dist class
 *
 *  This class is not normally used, as boost RNG is faster.
 */
  class Binomial_rnd_dist:public Rnd_dist {
    class Initializer {
    public:
      Initializer();
      std::vector < Real > logGammaTable;
    };

    friend class Initializer;

  public:
    Binomial_rnd_dist(Real pp, Size n);
    Binomial_rnd_dist(const Binomial_rnd_dist & original) = delete;
    const Binomial_rnd_dist & operator=(const Binomial_rnd_dist & right) = delete;
    virtual ~Binomial_rnd_dist();
    Real RndVal() const;
/** Identical to method 'bnldev' from
    Numerical Recipes in C (Press et al.) */
    Real Binomial(float pp, int n) const;
    Real LogGamma(Real val) const;

  protected:
    static bool init;

  private:
    Real p;
    Size n;
    static Initializer static_table;
  };
}  // namespace modigliani_base
#endif  // MODIGLIANI_MODIGLIANI_BASE_BINOMIAL_RND_DIST_H_
