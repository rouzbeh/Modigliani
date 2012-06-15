/**
 * @file ggaussian_rnd_dist.cc
 * Methods for gaussian random distribution class
 * @author Ahmed A. Faisal, 15. 10. 1998(c)
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

#include "gaussian_rnd_dist.h"

using namespace modigliani_base;

Gaussian_rnd_dist::Gaussian_rnd_dist() {
    mean = 0.0;
    stddevi = 1.0;
    stdNormSW = true;
}

Gaussian_rnd_dist::Gaussian_rnd_dist(Real newMean, Real newStddevi) {
    M_ASSERT(newStddevi >= 0.0);
    mean = newMean;
    stddevi = newStddevi;

    if ((0.0 == mean) && (1.0 == stddevi))
        stdNormSW = true;
    else
        stdNormSW = false;
}

Gaussian_rnd_dist::~Gaussian_rnd_dist() {
}

//
//    Method name : Gaussian_rnd_dist operator Real()
//
//    Description : "Cast" a random value to Real
//    Input : nothing
//    Output : an Real random value
//
Real Gaussian_rnd_dist::RndVal() const {
    if (true == stdNormSW)
        return ((Real) gaussian01());
    else
        return ((Real) gaussian01() * stddevi + mean);
}

//
//    Method name : gaussian01
//
//    Description : return gaussian random value with mean 0 and standard deviation (!) 1
//    Input :
//    Output : random value
//      Background : uses uniform1()
//
// See "float gasdev(long * idum)" from Numerical recipes in C, 2nd Edition, p. 289
//
float Gaussian_rnd_dist::gaussian01() const {
    static int iset = 0;
    static float gset;
    float fac, rsq, v1, v2;

    if (iset == 0) {
        do {
            v1 = 2.0 * uniformMT() - 1.0;
            v2 = 2.0 * uniformMT() - 1.0;
            rsq = v1 * v1 + v2 * v2;
        } while ((rsq >= 1.0) || (0.0 == rsq));
        fac = sqrt(-2.0 * log(rsq) / rsq);

        gset = v1 * fac;
        iset = 1;

        return (v2 * fac);
    } else {
        iset = 0;
        return (gset);
    }

}

