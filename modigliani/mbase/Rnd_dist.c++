/**
 * @file Rnd_dist.c++
 * Methods for random distribution super class
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

#include "Rnd_dist.h"

using namespace mbase;

long Rnd_dist::seed = (long) time(NULL); // 2DO improve initialization of seed
MTRand Rnd_dist::randgen = mbase::MMakeMTRandFunction(
		MTRand::uint32(~time(NULL)));

Rnd_dist::Rnd_dist() {
	if ((1.0 - 1.2e-7) == 1.0)
		std::cerr
				<< "Rnd_dist::Rnd_dist - Error : Insufficient floating point number accuracy (1.0 - 1.2e-7) == 1.0  in this implementation "
				<< std::endl;
}

Rnd_dist::~Rnd_dist() {
}

/** See "float ranqd2()" from Numerical recipes in C, 2nd Edition, p. 284 */
/** WARNING requires IEEE 32 bit representation of floats */
inline float Rnd_dist::uniformQuickDirty() const {
	static unsigned long jflone = 0x3f800000;
	static unsigned long jflmsk = 0x007fffff;

	static unsigned long idum;
	static unsigned long itemp;
	float randval;

	idum = 1664525L * idum + 1013904223L;
	itemp = jflone | (jflmsk & idum);
	randval = (*(float *) &itemp) - 1.0;

	return (randval);
}

//
//	Method name : uniform1
//
/**      return uniform random values within (0,1)
 Input : initialize idum once(!) to a negative integer
 Output : random value within (0,1)
 Background : Long period ( > 2 x 10^18 ) generator of L���Ecuyer with Bays-Durham
 shuffle and added safeguards
 Note: (1.0-1.2e-7) should approach the smallest number below 1.0

 See "float ran2(long * idum)" from Numerical recipes in C, 2nd Edition, p. 282
 */
inline float Rnd_dist::uniform1() const {
	int j;
	long k;
	static long idum2 = 123456789;
	static long iy = 0;
	static long iv[32];
	float temp;

	// cerr << "M_uniform_rnd_dist_o the seed is " << seed << endl;

	// init
	if (seed <= 0) {
		// prevent seed = 0
		if (-seed < 1)
			seed = 1;
		else
			seed = -seed;
		idum2 = seed;
		// load shuffle table after 8 warm-up loops
		for (j = 39; j >= 0; j--) {
			k = seed / 53668;
			seed = 40014 * (seed - k * 53668) - k * 12211;
			if (seed < 0)
				seed += 2147483563;
			if (j < 32)
				iv[j] = seed;
		}
		iy = iv[0];
	}
	// running mode (after init)
	k = seed / 53668;
	// execute the two random numbers
	// we use Schrage���s method two avoid overflow on 32-bit machienes
	seed = 40014 * (seed - k * 53668) - k * 12211;
	if (seed < 0)
		seed += 2147483563;
	k = idum2 / 52774;
	idum2 = 40692 * (idum2 - k * 52774) - k * 3791;
	if (idum2 < 0)
		idum2 += 2147483399;
	// the shuffle table index j will be always in 0..31 (proved)
	j = iy / (1 + 2147483562 / 32);
	// shuffle the seed, combine seed and idum2
	iy = iv[j] - idum2;
	iv[j] = seed;
	if (iy < 1)
		iy += 2147483562;
	// exclude endpoint value 1.0, so  ____________ should approach 1.0 -EPS != 1.0
	if ((temp = (1.0 / 2147483563) * iy) > (1.0 - 1.2e-7))
		return (1.0 - 1.2e-7);
	else
		return (temp);
}

