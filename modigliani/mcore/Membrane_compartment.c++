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
Membrane_compartment::Membrane_compartment(NTreal newArea /* in muMeter^2 */, NTreal newTemperature)
{
	NT_ASSERT( newArea > 0 );
	area = newArea;

	cM = 0;
	rA = 0;
	vM = 0;
	iInj = 0;
	temperature = newTemperature;
}

/* ***      COPY AND ASSIGNMENT	***/
Membrane_compartment::Membrane_compartment(const Membrane_compartment __attribute__((__unused__)) & original):
		Object()
{
 // should not be used
	cerr << "Membrane_compartment::Membrane_compartment(const .. & original)= - ERROR : Not implemented." << endl;
}

const Membrane_compartment&
Membrane_compartment::operator= (const Membrane_compartment & right)
{
 if (this == &right) return (*this); // Gracefully handle self assignment
 // should not be used
 cerr << "Membrane_compartment::operator= - ERROR : Not implemented." << endl;
  return (*this);
}

/* ***      DESTRUCTOR		***/
Membrane_compartment::~Membrane_compartment()
{
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
NTreturn
Membrane_compartment::step(NTreal newVM)
{
	vM = newVM;
	
	//for every current
	for (NTsize it = 0; it < currentVec.size(); it++) {
		(currentVec[it])->step(vM);
	}
	
	return (NT_SUCCESS);
}


/** @short
    @param      none
    @return     none
   \warning    OBSOLETE
   \bug        unknown
*/
NTreal
Membrane_compartment::total_conductance() const
{
	NTreal result = 0.0;
	vector< Membrane_current * >::const_iterator it = currentVec.begin();
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
NTreal
Membrane_compartment::WeightedConductance() const
{
	NTreal result = 0.0;
	
	vector< Membrane_current * >::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		result += ( (*it)->_conductance() ) * ( (*it)->_reversalPotential() );
	}
	return (result);
}

/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
*/
NTreturn
Membrane_compartment::AttachCurrent(Membrane_current * currentPtr, NTBPcurrentType type = NTBP_IONIC)
{
	currentPtr->Set_voltage(vM);
	currentPtr->setTimeStep(_timeStep());
	currentPtr->Set_temperature(temperature);
	switch (type){
		case NTBP_LEAK:
			currentVec.push_back(currentPtr);
		break;
		case NTBP_IONIC:
			currentVec.push_back(currentPtr);
			break;
		default:
			cerr <<"Membrane_compartment::AttachCurrent - Error : Unsupported current type "<< type <<"specified." << endl;
			return (NT_PARAM_UNSUPPORTED);
	}

	return (NT_SUCCESS);
}

NTreturn
Membrane_compartment::InjectCurrent(NTreal current /* in nA */)
{
//	NT_ASSERT(current >=0 ); 2DO is this necessary
	iInj = current;
	return (NT_SUCCESS);	
}

/** The total membrane capacitance of the compartment
	@return muF
*/
NTreal Membrane_compartment::CompartmentMembraneCapacitance() const
{
	return (cM /* muF/cm^2 */ * area /* muMeter^2 */ * 1.0e-8 /* cm^2/muMeter^2 */);
}

/** The net flowing current through the membrane of the compartment
	@return ?
*/
NTreal Membrane_compartment::CompartmentMembraneNetCurrent() const
{
	NTreal sumDeltaI = 0.0;
	for (NTsize it = 0; it < currentVec.size(); it++) {
			sumDeltaI -= (currentVec[it])->_current();	// i.e. ionic current is subtracted (modern  current convention)
	}
//	cerr << "\t Membrane capacitance [muF]"<< CompartmentMembraneCapacitance() << "\t Injected current [nA] "<< iInj <<  "\tionicCurrent="<<sumDeltaI << endl;	
	sumDeltaI += iInj; // the is  sign correct
	return (sumDeltaI);
}

/** Sum of escape rates from current state [1/kHz] */
NTreal Membrane_compartment::CompartmentChannelStateTimeConstant() const
{
  cerr <<"Membrane_compartment::CompartmentChannelStateTimeConstant" << endl;
	NTreal sum = 0;
	vector< Membrane_current * >::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
			//(*it)->ComputeRateConstants(_vM());
			sum += (*it)->ChannelStateTimeConstant();
			cerr << "Compartment Time constant sum=" << sum << endl;
	}


	return (sum);
}


bool Membrane_compartment::GillespieStep()
{
	cerr << "Membrane_compartment::GillespieStep()" << endl;
	NT_uniform_rnd_dist_o rnd;
	NTreal val = rnd.RndVal();
	NTreal sum = 0.0;		
	
  // 2DO this might be actually called more then once in one total iteration time step
	NTreal	compartmentTau = CompartmentChannelStateTimeConstant();
	
	cerr << "COMPARTMENT -> compartmentTau=" << compartmentTau << endl;
	vector< Membrane_current * >::iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
				sum += (*it)->ChannelStateTimeConstant();
				cerr << "COMPARTMENT -> SUM=" << sum  << " VAL=" << val <<  endl;
				if (val < sum/compartmentTau){
					return ((*it)->GillespieStep());
					break;
				}
	}
	cerr <<"Membrane_compartment::GillespieStep - Error : Control flow should not reach here." << endl;
	return (NT_FAIL);
}

void Membrane_compartment::ShowParam() const
{
	cout << "Compartment paramters:" << endl;
	cout << "Specific membrane capacitance [muF/cm^2] " << _cM() << endl;
	cout << "Axoplasmic resitance [Ohm cm] " << _rA() << endl;
	cout << "Membrane surface area [muMeter^2] " << _area() << endl;
	cout << "Compartment currents paramters:" << endl;
	vector< Membrane_current * >::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		(*it)->ShowParam();	
	}
}

vector< Membrane_current * > Membrane_compartment::ReturnCurrentVec ()
{
	auto cV = currentVec;
	return (cV);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
