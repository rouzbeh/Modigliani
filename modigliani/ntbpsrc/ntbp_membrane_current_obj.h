/**\file ntbp_membrane_current_obj.h - NTBP_membrane_current_o class header
 * by Ahmed Aldo Faisal &copy; created 16.3.2001
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

/* $Id: ntbp_membrane_current_obj.h,v 1.5 2003/01/30 17:13:43 face Exp $
 * $Log: ntbp_membrane_current_obj.h,v $
 * Revision 1.5  2003/01/30 17:13:43  face
 * *** empty log message ***
 *
 * Revision 1.4  2003/01/17 16:53:13  face
 * *** empty log message ***
 *
 * Revision 1.3  2002/03/13 19:23:27  face
 * *** empty log message ***
 *
 * Revision 1.2  2001/10/12 09:18:36  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#ifndef _ntbp_membrane_current_obj_h_
#define _ntbp_membrane_current_obj_h_

/* NT core includes */
#include "../ntsrc/nt_main.h"
#include "../ntsrc/nt_types.h"
#include "../ntsrc/nt_obj.h"
/* Parent includes */
#include "ntbp_object_obj.h"
/* NT includes */
#include "../ntsrc/nt_error_obj.h"
#include "../ntsrc/nt_uniform_rnd_dist_obj.h"
/* other includes */

/** @short NTBP_membrane_current_o class
 Represents the current accross a membrane,
 with its generalisable properties. It computes the
 actual currents based on the Nernst-Equation accross
 the membrane neglecting diffusion/concentration effects
 and the conductivity values provided by the hh_current
 or channel_current classes.
 \bug unknown
 \warning unknown
 */
class NTBP_membrane_current_o: public NTBP_object_o {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NTBP_membrane_current_o(NTreal reversalPotential /* in mV */);
	NTBP_membrane_current_o(const NTBP_membrane_current_o & original);
	const NTBP_membrane_current_o & operator=(
			const NTBP_membrane_current_o & right);
	virtual ~NTBP_membrane_current_o();

	/* ***  Methods              ***/
	/* in mSiemens/cm^2 */
	virtual NTreal _maxConductivity() const = 0;
	/** momentary current in nA */
	NTreal _current() const { /*cerr << "Current in nA" << current << endl;*/
		return (current);
	}
	/** momentary conductance in muFarad */
	NTreal _conductance() const {
		return (conductance);
	}
	/** reversal potential in mV */
	void Set_reversalPotential(NTreal eRev /* mV */) {
		reversalPotential = eRev;
	}
	/** reversal potential in mV */
	NTreal _reversalPotential() const {
		return (reversalPotential);
	}
	/** temperature in Celsius */
	NTreal _temperature() const {
		return (temperature);
	}
	/** Set temperature in Celsius */
	NTreturn Set_temperature(NTreal newTemp) {
		NT_ASSERT(newTemp > NT_0_KELVIN);
		temperature = newTemp;
		return (NT_SUCCESS);
	}
	/** Q10  */
	NTreal _q10() const {
		return (q10);
	}
	/** Set Q10 */
	virtual NTreturn Set_q10(NTreal newQ10) {
		q10 = newQ10;
		return (NT_SUCCESS);
	}
	/** Simulation mode */
	enum NTBPstochasticType _simulationMode() const {
		return (simulationMode);
	}
	/** Set simulation mode */
	virtual void SetSimulationMode(enum NTBPstochasticType newMode) {
		simulationMode = newMode;
	}

	NTreturn step(NTreal newVm /* in mV */) {
		//ComputeRateConstants(newVm); /* UpdateRateConstantsAND*/
		voltage = newVm;
		step_current();
		compute_conductance();
		compute_current(newVm);
		return (NT_SUCCESS);
	}
	/* in nA */
	NTreal compute_current(NTreal vM /* in mV */) {
		return (Set_current(
				_conductance() /* mSiemens */* 1000.0 /* mA/nA */* (vM
				/* mV */- _reversalPotential()/* mV */)));
	}
	virtual NTreturn step_current() = 0;
	/** compute and return conductance in mSiemens */
	virtual NTreal compute_conductance() = 0;
	/** compute the rate constants ( in ms^-1 ) */
	//virtual void ComputeRateConstants(NTreal vM /* in mV */) = 0;
	/** Number of open ionic channels */
	virtual NTreal open_channels() const = 0;
	/** Total number of ionic channels */
	virtual NTreal NumChannels() const {
		cerr
				<< "NTBP_membrane_current_o::NumChannels() - Error : Not Implemented."
				<< endl;
		return (-42);
	}

	virtual NTreal NumChannelsInState(NTsize __attribute__((__unused__)) state) const {
		cerr
				<< "NTBP_membrane_current_o::NumChannels() - Error : Not Implemented."
				<< endl;
		return (-42);
	}
	/** Number of open over total number of channels */
	virtual NTreal OpenChannelsRatio() const {
		cerr
				<< "NTBP_membrane_current_o::OpenChannelsRatio() - Error : Not Implemented."
				<< endl;
		return (-42);
	}
	virtual void ShowParam() const {
		cerr
				<< "NTBP_membrane_current_o::ShowParam - Warning : Not Implemented."
				<< endl;
	}
	/* Additional sweeps by the Gillespie Algorithm make the following method necessary */
	virtual NTreal ComputeChannelStateTimeConstant() const {
		cerr
				<< "NTBP_membrane_current_o::ComputeChannelStateTimeConstant - Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
				<< endl;
		return (0);
	}
	NTreal ChannelStateTimeConstant() const {
		return (ComputeChannelStateTimeConstant());
	}
	bool GillespieStep() {/*2DO is this necessary here*/
		step_current();
		return (ComputeGillespieStep());
	}
	virtual bool ComputeGillespieStep() {
		cerr
				<< "NTBP_membrane_current_o::ComputeGillespieStep- Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
				<< endl;
		return (NT_NOT_DERIVED);
	}

	NTreal Get_voltage() {
		return (voltage);
	}
	void Set_voltage(NTreal newVoltage) {
		voltage = newVoltage;
	}

	/* ***  Data                 ***/

protected:
	/* ***  Methods              ***/
	NTreal Set_current(NTreal newVal /* in nA */) {
		return (current = newVal);
	}
	NTreal Set_conductance(NTreal newVal /* in mSiemens */) {
		return (conductance = newVal);
	}
	/* ***  Data                 ***/
	NT_uniform_rnd_dist_o uniform;
	NTreal voltage;
	NTreal temperature; // in Celsius
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	NTreal current; // in nanoAmpere
	NTreal conductance; // in mSiemens
	NTreal reversalPotential; // in mV
	NTreal q10; // the Q_10 value for temperature dependent reaction kinetics
	enum NTBPstochasticType simulationMode;
};

#endif /* _ntbp_membrane_current_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
