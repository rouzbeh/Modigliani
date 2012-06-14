/**
 * @file Uniform_rnd_dist.c++
 * Methods for unfirom random distribution class
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

#include "Uniform_rnd_dist.h"

using namespace modigliani_base;
//
//	Method name : Uniform_rnd_dist
//
//	Description : CONSTRUCTORS
//	Input :
//	Output :
//

Uniform_rnd_dist::Uniform_rnd_dist() {
	a = 0;
	b = 1;

// cerr <<"Uniform_rnd_dist::Uniform_rnd_dist() - Warning : RndVal() uses quick and dirty algortihm for 32 bit machienes and low period."<< endl;
	set_statistics();
}

Uniform_rnd_dist::Uniform_rnd_dist(Real lowerBound,
		Real upperBound) {
	M_ASSERT(lowerBound < upperBound);

	a = lowerBound;
	b = upperBound;

	set_statistics();
}

//
//	Method name : Uniform_rnd_dist
//
//	Description : DESTRUCTOR
//	Input :
//	Output :
//
Uniform_rnd_dist::~Uniform_rnd_dist() {

}

inline Real Uniform_rnd_dist::RndVal() const {
	if (true == ab01)
		return ((Real) uniformMT());
	else
		return ((Real) (interval * uniformMT() + a));
//	if (true == ab01) return (Real) uniformQuickDirty();
//    else return (Real) ( interval*uniformQuickDirty() + a);
}

//
//	Method name : Uniform_rnd_dist
//
//	Description : "Cast" a random value in (a,b) to Real
//	Input : nothing
//	Output : an Real random value in (a,b)
//
Uniform_rnd_dist::operator Real() const {
	return (RndVal());
}

ReturnEnum Uniform_rnd_dist::ChangeBounds(Real lowerBound,
		Real upperBound) {
	if (lowerBound >= upperBound) {
		std::cerr << "Uniform_rnd_dist::ChangeBounds - Error : lowerBound "
				<< lowerBound
				<< "specified is equal or smaller than upperBound "
				<< upperBound << ". Bounds not changed." << std::endl;
		return (ReturnEnum::PARAM_OUT_OF_RANGE);
	}

	a = lowerBound;
	b = upperBound;

	set_statistics();
	return (ReturnEnum::SUCCESS);
}

//
//	Method nAMe : set_statistics
//
//	Description : set the statistical values according to (a,b),
//                    check whether to set the ab01-flag and compute the interval
//	Input : none
//	Output : none
//
void Uniform_rnd_dist::set_statistics() {
	//
	if (a == 0 && b == 1)
		ab01 = true;
	else
		ab01 = false;

	mean = (a + b) / 2.0;
	variance = fabs(a - b);

	interval = (fabs(a) + fabs(b));
}

