/**\file Leak_current.h - Leak_current class header
 * by Ahmed Aldo Faisal &copy; created 19.3.2001  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mcore_leak_current_h_
#define _mcore_leak_current_h_

#include "Membrane_current.h"

/** @short Leak_current class
 \bug unknown
 \warning unknown
 */
namespace mcore {
class Leak_current: public Membrane_current {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Leak_current(mbase::Real newArea /* in mu^2 */,
			mbase::Real newLeakConductance = 0.3 /* mS/cm^2 */,
			mbase::Real newReversalPotential = 10.613 /* mV */);
	Leak_current(const Leak_current & original);
	const Leak_current & operator=(const Leak_current & right);
	virtual ~Leak_current();
	/* ***  Methods              ***/
	void ComputeRateConstants(
			mbase::Real __attribute__((__unused__)) _voltage /* in mV */) {
	}
	/** in mSiemens */
	mbase::Mreturn step_current() {
		return (mbase::M_SUCCESS);
	}
	/** compute and return conductance in mSiemens */
	void UpdateConductance() {
		Set_conductance(_maxConductivity() * _area() * 1.0e-8);
	}
	/** Return leak conductance in mSiemens  (note: function return constant value (leak!) set in constructor) */
	mbase::Real compute_conductance() {
		return (_conductance());
	}
	mbase::Mreturn DeterministicStepCurrent() {
		return (mbase::M_SUCCESS);
	}
	void Show() {
		std::cout << "g_Leak [mSiemens]=" << _conductance();
	}
	/** Number of open ionic channels */
	virtual mbase::Real open_channels() const {
		return (0.0);
	}
	/** Total number of ionic channels */
	virtual mbase::Real NumChannels() const {
		return (0.0);
	}
	/** Number of open over total number of channels */
	virtual mbase::Real OpenChannelsRatio() const {
		return (0.0);
	}

	virtual mbase::Real NumChannelsInState(
			mbase::Size_t __attribute__((__unused__)) state) const {
		return (0);
	}
	mbase::Real ComputeChannelStateTimeConstant() const {
		return (0); /* no stochasticity */
	}
	/** in mSiemens/cm^2 */
	mbase::Real _maxConductivity() const {
		return (maxConductivity);
	}
	/** in muMeter^2 */
	mbase::Real _area() const {
		return (area);
	}
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	mbase::Real maxConductivity; // in mSiemens / muMeter^2
	mbase::Real density; // channels per mumeter^2
	mbase::Real area; // in mumeter^2
	mbase::Real conductivity; // in mSiemens per channel
};
}
#endif /* _mcore_leak_current_h_ */

/* File skeleton generated by GenNTObj version 0.7. */