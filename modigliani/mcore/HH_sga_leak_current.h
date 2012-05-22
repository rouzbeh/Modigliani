/**\file HH_sga_leak_current.h - HH_sga_leak_current_o class header
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

/* $Id: ntbp_hh_sga_leak_current.h,v 1.2 2005/04/25 13:55:27 face Exp $ 
 * $Log: ntbp_hh_sga_leak_current.h,v $
 * Revision 1.2  2005/04/25 13:55:27  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2003/08/14 16:43:27  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#ifndef _mcore_hh_sga_leak_current_h_
#define _mcore_hh_sga_leak_current_h_

#include "Object.h"
/* Parent includes */
#include "HH_current.h"
/* NT includes */

/* other includes */

/** @short HH_sga_leak_current_o class
 \bug unknown
 \warning unknown
 */
namespace mcore{
class HH_sga_leak_current: public HH_current {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	HH_sga_leak_current(mbase::Real newArea /* in mu^2 */,
			mbase::Real newLeakConductance = 0.3 /* mS/cm^2 */,
			mbase::Real newReversalPotential = 10.613 /* mV */);
	HH_sga_leak_current(const HH_sga_leak_current & original);
	const HH_sga_leak_current & operator=(
			const HH_sga_leak_current & right);
	virtual ~HH_sga_leak_current();
	/* ***  Methods              ***/
	void ComputeRateConstants(mbase::Real __attribute__((__unused__)) _voltage /* in mV */) {
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

	virtual mbase::Real NumChannelsInState(mbase::Msize __attribute__((__unused__)) state) const {
		return (0);
	}
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
};
}
#endif /* _mcore_hh_sga_leak_current.h_ */

/* File skeleton generated by GenNTObj version 0.7. */
