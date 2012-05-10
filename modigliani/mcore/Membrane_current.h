/**\file Membrane_current.h - Mmbrane_current_o class header
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

#ifndef _mcore_membrane_current_h_
#define _mcore_membrane_current_h_

/* NT core includes */
#include "mbase/main.h"
#include "mbase/types.h"
/* Parent includes */
#include "Object.h"
/* NT includes */
#include "mbase/Uniform_rnd_dist.h"
/* other includes */

/** @short membrane_current_o class
 Represents the current accross a membrane,
 with its generalisable properties. It computes the
 actual currents based on the Nernst-Equation accross
 the membrane neglecting diffusion/concentration effects
 and the conductivity values provided by the hh_current
 or channel_current classes.
 \bug unknown
 \warning unknown
 */

namespace mcore {
class Membrane_current: public Object {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Membrane_current(mbase::Mreal reversalPotential /* in mV */);
	Membrane_current(const Membrane_current & original);
	const Membrane_current & operator=(const Membrane_current & right);
	virtual ~Membrane_current();

	/* ***  Methods              ***/
	/* in mSiemens/cm^2 */
	virtual mbase::Mreal _maxConductivity() const = 0;
	/** momentary current in nA */
	mbase::Mreal _current() const { /*std::cerr << "Current in nA" << current << std::endl;*/
		return (current);
	}
	/** momentary conductance in muFarad */
	mbase::Mreal _conductance() const {
		return (conductance);
	}
	/** reversal potential in mV */
	void Set_reversalPotential(mbase::Mreal eRev /* mV */) {
		reversalPotential = eRev;
	}
	/** reversal potential in mV */
	mbase::Mreal _reversalPotential() const {
		return (reversalPotential);
	}
	/** temperature in Celsius */
	mbase::Mreal _temperature() const {
		return (temperature);
	}
	/** Set temperature in Celsius */
	mbase::Mreturn Set_temperature(mbase::Mreal newTemp) {
		M_ASSERT(newTemp > mbase::ZERO_KELVIN);
		temperature = newTemp;
		return (mbase::M_SUCCESS);
	}
	/** Q10  */
	mbase::Mreal _q10() const {
		return (q10);
	}
	/** Set Q10 */
	virtual mbase::Mreturn Set_q10(mbase::Mreal newQ10) {
		q10 = newQ10;
		return (mbase::M_SUCCESS);
	}
	/** Simulation mode */
	enum NTBPstochasticType _simulationMode() const {
		return (simulationMode);
	}
	/** Set simulation mode */
	virtual void SetSimulationMode(enum NTBPstochasticType newMode) {
		simulationMode = newMode;
	}

	mbase::Mreturn step(mbase::Mreal newVm /* in mV */) {
		//ComputeRateConstants(newVm); /* UpdateRateConstantsAND*/
		voltage = newVm;
		step_current();
		compute_conductance();
		compute_current(newVm);
		return (mbase::M_SUCCESS);
	}
	/* in nA */
	mbase::Mreal compute_current(mbase::Mreal vM /* in mV */) {
		return (Set_current(
				_conductance() /* mSiemens */* 1000.0 /* mA/nA */* (vM
				/* mV */- _reversalPotential()/* mV */)));
	}
	virtual mbase::Mreturn step_current() = 0;
	/** compute and return conductance in mSiemens */
	virtual mbase::Mreal compute_conductance() = 0;
	/** compute the rate constants ( in ms^-1 ) */
	//virtual void ComputeRateConstants(mbase::Mreal vM /* in mV */) = 0;
	/** Number of open ionic channels */
	virtual mbase::Mreal open_channels() const = 0;
	/** Total number of ionic channels */
	virtual mbase::Mreal NumChannels() const {
		std::cerr
				<< "NTBP_membrane_current_o::NumChannels() - Error : Not Implemented."
				<< std::endl;
		return (-42);
	}

	virtual mbase::Mreal NumChannelsInState(
			mbase::Msize __attribute__((__unused__)) state) const {
		std::cerr
				<< "NTBP_membrane_current_o::NumChannels() - Error : Not Implemented."
				<< std::endl;
		return (-42);
	}
	/** Number of open over total number of channels */
	virtual mbase::Mreal OpenChannelsRatio() const {
		std::cerr
				<< "NTBP_membrane_current_o::OpenChannelsRatio() - Error : Not Implemented."
				<< std::endl;
		return (-42);
	}
	virtual void ShowParam() const {
		std::cerr
				<< "NTBP_membrane_current_o::ShowParam - Warning : Not Implemented."
				<< std::endl;
	}
	/* Additional sweeps by the Gillespie Algorithm make the following method necessary */
	virtual mbase::Mreal ComputeChannelStateTimeConstant() const {
		std::cerr
				<< "NTBP_membrane_current_o::ComputeChannelStateTimeConstant - Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
				<< std::endl;
		return (0);
	}
	mbase::Mreal ChannelStateTimeConstant() const {
		return (ComputeChannelStateTimeConstant());
	}
	bool GillespieStep() {/*2DO is this necessary here*/
		step_current();
		return (ComputeGillespieStep());
	}
	virtual bool ComputeGillespieStep() {
		std::cerr
				<< "NTBP_membrane_current_o::ComputeGillespieStep- Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
				<< std::endl;
		return (mbase::M_NOT_DERIVED);
	}

	mbase::Mreal Get_voltage() {
		return (voltage);
	}
	void Set_voltage(mbase::Mreal newVoltage) {
		voltage = newVoltage;
	}

	/* ***  Data                 ***/

protected:
	/* ***  Methods              ***/
	mbase::Mreal Set_current(mbase::Mreal newVal /* in nA */) {
		return (current = newVal);
	}
	mbase::Mreal Set_conductance(mbase::Mreal newVal /* in mSiemens */) {
		return (conductance = newVal);
	}
	/* ***  Data                 ***/
	mbase::Uniform_rnd_dist uniform;
	mbase::Mreal voltage;
	mbase::Mreal temperature; // in Celsius
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	mbase::Mreal current; // in nanoAmpere
	mbase::Mreal conductance; // in mSiemens
	mbase::Mreal reversalPotential; // in mV
	mbase::Mreal q10; // the Q_10 value for temperature dependent reaction kinetics
	enum NTBPstochasticType simulationMode;
};
}
#endif /* _mcore_membrane_current.h_ */

/* File skeleton generated by GenNTObj version 0.7. */
