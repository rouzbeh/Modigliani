/* Uniform_rnd_dist.h - Header for unfirom random distribution class */
/* by Ahmed A. Faisal, 15. 10. 1998(c) */
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
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mbase_uniform_rnd_dist_h_
#define _mbase_uniform_rnd_dist_h_

#include "Rnd_dist.h"
#include "Vector.h"

//
//	Class name : Uniform_rnd_dist
//
namespace mbase {
class Uniform_rnd_dist: public Rnd_dist {
public:
	Uniform_rnd_dist();
	// Uniform_rnd_dist(Mint dim);
	Uniform_rnd_dist(Mreal lowerBound, Mreal upperBound);
	// Uniform_rnd_dist(Mreal lowerBound, Mreal upperBound, Mint dim);
	Uniform_rnd_dist(const Uniform_rnd_dist &);
	~Uniform_rnd_dist();

	Mreal RndVal() const;
	operator Mreal() const;
	Mreturn ChangeBounds(Mreal lowerBound, Mreal upperBound);
	// operator M_vector_o() const;

	Mreal _a() const {
		return (a);
	}
	;
	Mreal _b() const {
		return (b);
	}
	;
	Mreal _mean() const {
		return (mean);
	}
	;
	Mreal _variance() const {
		return (variance);
	}
	;
	// Mint _dimension() const   { return dimension; };
	friend std::ostream & operator<<(std::ostream & os, const Uniform_rnd_dist & self);

private:
	void set_statistics();
	Mreal a;
	Mreal b;
	Mreal interval;
	Muint dimension;
	Mbool ab01; // faster-(0,1)-call flag
};
}

#endif /* _mbase_uniform_rnd_dist_h_ */

