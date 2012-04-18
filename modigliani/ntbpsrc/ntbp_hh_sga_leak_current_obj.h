/**\file ntbp_hh_sga_leak_current_obj.h - NTBP_hh_sga_leak_current_o class header 
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

/* $Id: ntbp_hh_sga_leak_current_obj.h,v 1.2 2005/04/25 13:55:27 face Exp $ 
 * $Log: ntbp_hh_sga_leak_current_obj.h,v $
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
#ifndef _ntbp_hh_sga_leak_current_obj_h_ 
#define _ntbp_hh_sga_leak_current_obj_h_ 

/* NT core includes */
#include "ntsrc/nt_main.h" 
#include "ntsrc/nt_types.h" 
#include "ntsrc/nt_obj.h"  
/* Parent includes */
#include "ntbp_hh_current_obj.h" 
/* NT includes */

/* other includes */

/** @short NTBP_hh_sga_leak_current_o class 
 \bug unknown
 \warning unknown
 */
class NTBP_hh_sga_leak_current_o: public NTBP_hh_current_o {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NTBP_hh_sga_leak_current_o(NTreal newArea /* in mu^2 */,
			NTreal newLeakConductance = 0.3 /* mS/cm^2 */,
			NTreal newReversalPotential = 10.613 /* mV */);
	NTBP_hh_sga_leak_current_o(const NTBP_hh_sga_leak_current_o & original);
	const NTBP_hh_sga_leak_current_o & operator=(
			const NTBP_hh_sga_leak_current_o & right);
	virtual ~NTBP_hh_sga_leak_current_o();
	/* ***  Methods              ***/
	void ComputeRateConstants(NTreal __attribute__((__unused__)) _voltage /* in mV */) {
	}
	/** in mSiemens */
	NTreturn step_current() {
		return (NT_SUCCESS);
	}
	/** compute and return conductance in mSiemens */
	void UpdateConductance() {
		Set_conductance(_maxConductivity() * _area() * 1.0e-8);
	}
	/** Return leak conductance in mSiemens  (note: function return constant value (leak!) set in constructor) */
	NTreal compute_conductance() {
		return (_conductance());
	}
	NTreturn DeterministicStepCurrent() {
		return (NT_SUCCESS);
	}
	void Show() {
		cout << "g_Leak [mSiemens]=" << _conductance();
	}
	/** Number of open ionic channels */
	virtual NTreal open_channels() const {
		return (0.0);
	}
	/** Total number of ionic channels */
	virtual NTreal NumChannels() const {
		return (0.0);
	}
	/** Number of open over total number of channels */
	virtual NTreal OpenChannelsRatio() const {
		return (0.0);
	}

	virtual NTreal NumChannelsInState(NTsize __attribute__((__unused__)) state) const {
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
#endif /* _ntbp_hh_sga_leak_current_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
