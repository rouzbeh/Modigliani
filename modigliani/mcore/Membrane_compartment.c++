/**\file Membrane_compartmentbj.cpp - Membrane_compartment class implementation
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

#include "Membrane_compartment.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Membrane_compartment */
Membrane_compartment::Membrane_compartment(
		mbase::Real newArea /* in muMeter^2 */, mbase::Real newTemperature) {
	M_ASSERT( newArea > 0);
	area = newArea;

	cM = 0;
	rA = 0;
	vM = 0;
	iInj = 0;
	temperature = newTemperature;
}

/* ***      COPY AND ASSIGNMENT	***/
Membrane_compartment::Membrane_compartment(
		const Membrane_compartment __attribute__((__unused__)) & original) :
		Object() {
	// should not be used
	std::cerr
			<< "Membrane_compartment::Membrane_compartment(const .. & original)= - ERROR : Not implemented."
			<< std::endl;
}

Membrane_compartment&
Membrane_compartment::operator=(const Membrane_compartment & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// should not be used
	std::cerr << "Membrane_compartment::operator= - ERROR : Not implemented."
			<< std::endl;
	return (*this);
}

/* ***      DESTRUCTOR		***/
Membrane_compartment::~Membrane_compartment() {
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
mbase::Mreturn Membrane_compartment::step(mbase::Real newVM) {
	vM = newVM;

	//for every current
	for (mbase::Size_t it = 0; it < currentVec.size(); it++) {
		(currentVec[it])->step(vM);
	}

	return (mbase::M_SUCCESS);
}

/** @short
 @param      none
 @return     none
 \warning    OBSOLETE
 \bug        unknown
 */
mbase::Real Membrane_compartment::total_conductance() const {
	mbase::Real result = 0.0;
	std::vector<Membrane_current *>::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		result += (*it)->_conductance();
	}
	return (result);
}

/** @short Conductance weighted with reversal potential
 @param      none
 @return     none
 \warning    OBSOLETE ?
 \bug        unknown
 */
mbase::Real Membrane_compartment::WeightedConductance() const {
	mbase::Real result = 0.0;

	std::vector<Membrane_current *>::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		result += ((*it)->_conductance()) * ((*it)->_reversalPotential());
	}
	return (result);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
mbase::Mreturn Membrane_compartment::AttachCurrent(
		Membrane_current * currentPtr, NTBPcurrentType type = NTBP_IONIC) {
	currentPtr->Set_voltage(vM);
	currentPtr->setTimeStep(_timeStep());
	currentPtr->Set_temperature(temperature);
	switch (type) {
	case NTBP_LEAK:
		currentVec.push_back(currentPtr);
		break;
	case NTBP_IONIC:
		currentVec.push_back(currentPtr);
		break;
	default:
		std::cerr
				<< "Membrane_compartment::AttachCurrent - Error : Unsupported current type "
				<< type << "specified." << std::endl;
		return (mbase::M_PARAM_UNSUPPORTED);
	}

	return (mbase::M_SUCCESS);
}

mbase::Mreturn Membrane_compartment::InjectCurrent(
		mbase::Real current /* in nA */) {
//	M_ASSERT(current >=0 ); 2DO is this necessary
	iInj = current;
	return (mbase::M_SUCCESS);
}

/** The total membrane capacitance of the compartment
 @return muF
 */
mbase::Real Membrane_compartment::CompartmentMembraneCapacitance() const {
	return (cM /* muF/cm^2 */* area /* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
}

/** The net flowing current through the membrane of the compartment
 @return ?
 */
mbase::Real Membrane_compartment::CompartmentMembraneNetCurrent() const {
	mbase::Real sumDeltaI = 0.0;
	for (mbase::Size_t it = 0; it < currentVec.size(); it++) {
		sumDeltaI -= (currentVec[it])->_current(); // i.e. ionic current is subtracted (modern  current convention)
	}
//	std::cerr << "\t Membrane capacitance [muF]"<< CompartmentMembraneCapacitance() << "\t Injected current [nA] "<< iInj <<  "\tionicCurrent="<<sumDeltaI << std::endl;	
	sumDeltaI += iInj; // the is  sign correct
	return (sumDeltaI);
}

/** Sum of escape rates from current state [1/kHz] */
mbase::Real Membrane_compartment::CompartmentChannelStateTimeConstant() const {
	std::cerr << "Membrane_compartment::CompartmentChannelStateTimeConstant"
			<< std::endl;
	mbase::Real sum = 0;
	std::vector<Membrane_current *>::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		//(*it)->ComputeRateConstants(_vM());
		sum += (*it)->ChannelStateTimeConstant();
		std::cerr << "Compartment Time constant sum=" << sum << std::endl;
	}

	return (sum);
}

bool Membrane_compartment::GillespieStep() {
	std::cerr << "Membrane_compartment::GillespieStep()" << std::endl;
	mbase::Uniform_rnd_dist rnd;
	mbase::Real val = rnd.RndVal();
	mbase::Real sum = 0.0;

	// 2DO this might be actually called more then once in one total iteration time step
	mbase::Real compartmentTau = CompartmentChannelStateTimeConstant();

	std::cerr << "COMPARTMENT -> compartmentTau=" << compartmentTau
			<< std::endl;
	std::vector<Membrane_current *>::iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		sum += (*it)->ChannelStateTimeConstant();
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
	return (mbase::M_FAIL);
}

void Membrane_compartment::show_param() const {
	std::cout << "Compartment paramters:" << std::endl;
	std::cout << "Specific membrane capacitance [muF/cm^2] " << _cM()
			<< std::endl;
	std::cout << "Axoplasmic resitance [Ohm cm] " << _rA() << std::endl;
	std::cout << "Membrane surface area [muMeter^2] " << _area() << std::endl;
	std::cout << "Compartment currents paramters:" << std::endl;
	std::vector<Membrane_current *>::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		(*it)->show_param();
	}
}

std::vector<Membrane_current *> Membrane_compartment::ReturnCurrentVec() {
	auto cV = currentVec;
	return (cV);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
