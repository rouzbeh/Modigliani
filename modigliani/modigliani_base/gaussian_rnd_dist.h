/**
 * @file gaussian_rnd_dist.h
 * Header for vectorial gaussian random distribution class
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

#ifndef _modigliani_base_gaussian_rnd_dist_h_
#define _modigliani_base_gaussian_rnd_dist_h_

#include "rnd_dist.h"
#include "vector.h"

namespace modigliani_base{
/**    Gaussian_rnd_dist returns values out of a gaussian
                    random distribution objects based upon inidividual paramters
              for each subspace
    Idea : The usage idea is to call the object which returns
             a Real or M_vector_o containing random values out
             of its specific distribution [a,b]
         */
class Gaussian_rnd_dist : public Rnd_dist {
public:
    Gaussian_rnd_dist();
    Gaussian_rnd_dist(Real newMean, Real newStdDev);
    Gaussian_rnd_dist(const Gaussian_rnd_dist &);
    virtual ~Gaussian_rnd_dist();
    operator Real() const {
        return (RndVal());
    }
    Real RndVal() const;
    Real _stddevi() const  {
        return (stddevi);
    };
    friend std::ostream & operator<<(std::ostream & os, const Gaussian_rnd_dist & self);


protected:
    float gaussian01() const;

private:
    //! switch for faster standard normal distribution computation
    bool stdNormSW;
    Real stddevi;
};
}

#endif /* _modigliani_base_gaussian_rnd_dist_h_ */

