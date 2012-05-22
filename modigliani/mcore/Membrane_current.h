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
	Membrane_current(mbase::Real reversalPotential /* in mV */);
	Membrane_current(const Membrane_current & original);
	const Membrane_current & operator=(const Membrane_current & right);
	virtual ~Membrane_current();

	/* ***  Methods              ***/
	/* in mSiemens/cm^2 */
	virtual mbase::Real _maxConductivity() const = 0;
	/** momentary current in nA */
	mbase::Real _current() const { /*std::cerr << "Current in nA" << current << std::endl;*/
		return (current);
	}
	/** momentary conductance in muFarad */
	mbase::Real _conductance() const {
		return (conductance);
	}
	/** reversal potential in mV */
	void Set_reversalPotential(mbase::Real eRev /* mV */) {
		reversalPotential = eRev;
	}
	/** reversal potential in mV */
	mbase::Real _reversalPotential() const {
		return (reversalPotential);
	}
	/** temperature in Celsius */
	mbase::Real _temperature() const {
		return (temperature);
	}
	/** Set temperature in Celsius */
	mbase::Mreturn Set_temperature(mbase::Real newTemp) {
		M_ASSERT(newTemp > mbase::ZERO_KELVIN);
		temperature = newTemp;
		return (mbase::M_SUCCESS);
	}
	/** Q10  */
	mbase::Real _q10() const {
		return (q10);
	}
	/** Set Q10 */
	virtual mbase::Mreturn Set_q10(mbase::Real newQ10) {
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

	mbase::Mreturn step(mbase::Real newVm /* in mV */) {
		//ComputeRateConstants(newVm); /* UpdateRateConstantsAND*/
		voltage = newVm;
		step_current();
		compute_conductance();
		compute_current(newVm);
		return (mbase::M_SUCCESS);
	}
	/* in nA */
	mbase::Real compute_current(mbase::Real vM /* in mV */) {
		return (Set_current(
				_conductance() /* mSiemens */* 1000.0 /* mA/nA */* (vM
				/* mV */- _reversalPotential()/* mV */)));
	}
	virtual mbase::Mreturn step_current() = 0;
	/** compute and return conductance in mSiemens */
	virtual mbase::Real compute_conductance() = 0;
	/** compute the rate constants ( in ms^-1 ) */
	//virtual void ComputeRateConstants(mbase::Real vM /* in mV */) = 0;
	/** Number of open ionic channels */
	virtual mbase::Real open_channels() const = 0;
	/** Total number of ionic channels */
	virtual mbase::Real NumChannels() const {
		std::cerr
				<< "NTBP_membrane_current_o::NumChannels() - Error : Not Implemented."
				<< std::endl;
		return (-42);
	}

	virtual mbase::Real num_channels_in_state(
			mbase::Msize __attribute__((__unused__)) state) const {
		std::cerr
				<< "NTBP_membrane_current_o::NumChannels() - Error : Not Implemented."
				<< std::endl;
		return (-42);
	}
	/** Number of open over total number of channels */
	virtual mbase::Real OpenChannelsRatio() const {
		std::cerr
				<< "NTBP_membrane_current_o::OpenChannelsRatio() - Error : Not Implemented."
				<< std::endl;
		return (-42);
	}
	virtual void show_param() const {
		std::cerr
				<< "NTBP_membrane_current_o::ShowParam - Warning : Not Implemented."
				<< std::endl;
	}
	/* Additional sweeps by the Gillespie Algorithm make the following method necessary */
	virtual mbase::Real ComputeChannelStateTimeConstant() const {
		std::cerr
				<< "NTBP_membrane_current_o::ComputeChannelStateTimeConstant - Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
				<< std::endl;
		return (0);
	}
	mbase::Real ChannelStateTimeConstant() const {
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

	mbase::Real Get_voltage() {
		return (voltage);
	}
	void Set_voltage(mbase::Real newVoltage) {
		voltage = newVoltage;
	}

	/* ***  Data                 ***/

protected:
	/* ***  Methods              ***/
	mbase::Real Set_current(mbase::Real newVal /* in nA */) {
		return (current = newVal);
	}
	mbase::Real Set_conductance(mbase::Real newVal /* in mSiemens */) {
		return (conductance = newVal);
	}
	/* ***  Data                 ***/
	mbase::Uniform_rnd_dist uniform;
	mbase::Real voltage;
	mbase::Real temperature; // in Celsius
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	mbase::Real current; // in nanoAmpere
	mbase::Real conductance; // in mSiemens
	mbase::Real reversalPotential; // in mV
	mbase::Real q10; // the Q_10 value for temperature dependent reaction kinetics
	enum NTBPstochasticType simulationMode;
};
}
#endif /* _mcore_membrane_current.h_ */

/* File skeleton generated by GenNTObj version 0.7. */
