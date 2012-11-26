/**
 * @file membrane_compartmentbj.cc
 * Membrane_compartment class implementation
 * @author Ahmed Aldo Faisal &copy; created 16.3.2001
 * @version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * @section LICENSE
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

#include "membrane_compartment.h"

using namespace modigliani_core;

/* ***      CONSTRUCTORS	***/
/** Create a Membrane_compartment */
Membrane_compartment::Membrane_compartment(
		modigliani_base::Real newArea /* in muMeter^2 */, modigliani_base::Real newTemperature) {
	M_ASSERT( newArea > 0);
	area_ = newArea;

	cm_ = 0;
	ra_ = 0;
	vm_ = 0;
	i_inj_ = 0;
	temperature_ = newTemperature;
}

/* ***      DESTRUCTOR		***/
Membrane_compartment::~Membrane_compartment() {
	for (auto it = current_vec_.begin(); it != current_vec_.end(); ++it) {
		delete *it;
	}
}

/* ***  PUBLIC                                    ***   */
/** @short      The membrane compartment has one modes of operation.
 If the voltage is specified externally at each time step,
 the compartment acts as a container for the occuring current
 objects, integratios has to be provided externally.
 e.g.
 Membrane_compartment c; [...]
 FLOAT voltage;
 LOOP
 voltage = IntegrateDifferentialEquation( c._vM() );
 c.Step( voltage );
 END LOOP;
 Additionally single compartment HodgkinHuxley simulation is possible using:
 where the voltage is updated at each step, if the
 old voltage is specified as input:
 e.g. LOOP
 Membrane_compartment c; [...]
 c.Step( c._vM() );	deltaVM = 1.0e-3 / sec /mSec / * CompartmentMembraneNetCurrent() / nA / /CompartmentMembraneCapacitance() / muF /;
 vM += deltaVM * _timeStep();
 END LOOP;
 @param      none
 @return     none
 \warning    Voltage is updated at Step
 \bug        unknown
 */
modigliani_base::ReturnEnum Membrane_compartment::step(modigliani_base::Real newVM) {
	vm_ = newVM;

	//for every current
	for (modigliani_base::Size it = 0; it < current_vec_.size(); it++) {
		(current_vec_[it])->Step(vm_);
	}

	return (modigliani_base::ReturnEnum::SUCCESS);
}

/** @short
 @param      none
 @return     none
 \warning    OBSOLETE
 \bug        unknown
 */
modigliani_base::Real Membrane_compartment::total_conductance() const {
	modigliani_base::Real result = 0.0;
	std::vector<Membrane_current *>::const_iterator it = current_vec_.begin();
	for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
		result += (*it)->conductance();
	}
	return (result);
}

/** @short Conductance weighted with reversal potential
 @param      none
 @return     none
 \warning    OBSOLETE ?
 \bug        unknown
 */
modigliani_base::Real Membrane_compartment::WeightedConductance() const {
	modigliani_base::Real result = 0.0;

	std::vector<Membrane_current *>::const_iterator it = current_vec_.begin();
	for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
		result += ((*it)->conductance()) * ((*it)->reversal_potential());
	}
	return (result);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
modigliani_base::ReturnEnum Membrane_compartment::AttachCurrent(
		Membrane_current * currentPtr, NTBPcurrentType type){
	currentPtr->set_voltage(vm_);
	currentPtr->setTimeStep(_timeStep());
	currentPtr->set_temperature(temperature_);
	switch (type) {
	case NTBP_LEAK:
		current_vec_.push_back(currentPtr);
		break;
	case NTBP_IONIC:
		current_vec_.push_back(currentPtr);
		break;
	default:
		std::cerr
				<< "Membrane_compartment::AttachCurrent - Error : Unsupported current type "
				<< type << "specified." << std::endl;
		return (modigliani_base::ReturnEnum::PARAM_UNSUPPORTED);
	}

	return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Membrane_compartment::InjectCurrent(
		modigliani_base::Real current /* in nA */) {
//	M_ASSERT(current >=0 ); 2DO is this necessary
	i_inj_ = current;
	return (modigliani_base::ReturnEnum::SUCCESS);
}

/** The total membrane capacitance of the compartment
 @return muF
 */
modigliani_base::Real Membrane_compartment::CompartmentMembraneCapacitance() const {
	return (cm_ /* muF/cm^2 */* area_ /* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
}

/** The net flowing current through the membrane of the compartment
 @return ?
 */
modigliani_base::Real Membrane_compartment::CompartmentMembraneNetCurrent() const {
	modigliani_base::Real sumDeltaI = 0.0;
	for (modigliani_base::Size it = 0; it < current_vec_.size(); it++) {
		sumDeltaI -= (current_vec_[it])->current(); // i.e. ionic current is subtracted (modern  current convention)
	}
//	std::cerr << "\t Membrane capacitance [muF]"<< CompartmentMembraneCapacitance() << "\t Injected current [nA] "<< iInj <<  "\tionicCurrent="<<sumDeltaI << std::endl;	
	sumDeltaI += i_inj_; // the is  sign correct
	return (sumDeltaI);
}

/** Sum of escape rates from current state [1/kHz] */
modigliani_base::Real Membrane_compartment::CompartmentChannelStateTimeConstant() const {
	std::cerr << "Membrane_compartment::CompartmentChannelStateTimeConstant"
			<< std::endl;
	modigliani_base::Real sum = 0;
	std::vector<Membrane_current *>::const_iterator it = current_vec_.begin();
	for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
		//(*it)->ComputeRateConstants(_vM());
		sum += (*it)->ComputeTimeConstant();
		std::cerr << "Compartment Time constant sum=" << sum << std::endl;
	}

	return (sum);
}

bool Membrane_compartment::GillespieStep() {
	std::cerr << "Membrane_compartment::GillespieStep()" << std::endl;
	modigliani_base::Uniform_rnd_dist rnd;
	modigliani_base::Real val = rnd.RndVal();
	modigliani_base::Real sum = 0.0;

	// 2DO this might be actually called more then once in one total iteration time step
	modigliani_base::Real compartmentTau = CompartmentChannelStateTimeConstant();

	std::cerr << "COMPARTMENT -> compartmentTau=" << compartmentTau
			<< std::endl;
	std::vector<Membrane_current *>::iterator it = current_vec_.begin();
	for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
		sum += (*it)->ComputeTimeConstant();
		std::cerr << "COMPARTMENT -> SUM=" << sum << " VAL=" << val
				<< std::endl;
		if (val < sum / compartmentTau) {
			return ((*it)->GillespieStep());
			break;
		}
	}
	std::cerr
			<< "Membrane_compartment::GillespieStep - Error : Control flow should not reach here."
			<< std::endl;
	return (modigliani_base::ReturnEnum::FAIL);
}

void Membrane_compartment::show_param() const {
	std::cout << "Compartment paramters:" << std::endl;
	std::cout << "Specific membrane capacitance [muF/cm^2] " << cm()
			<< std::endl;
	std::cout << "Axoplasmic resitance [Ohm cm] " << ra() << std::endl;
	std::cout << "Membrane surface area [muMeter^2] " << area() << std::endl;
	std::cout << "Compartment currents paramters:" << std::endl;
	std::vector<Membrane_current *>::const_iterator it = current_vec_.begin();
	for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
		(*it)->show_param();
	}
}

modigliani_base::Size Membrane_compartment::NumberCurrents() const {
	return (current_vec_.size());
}

Membrane_current const * Membrane_compartment::GetCurrent(
		modigliani_base::Size i) const {
	return (current_vec_[i - 1]);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
