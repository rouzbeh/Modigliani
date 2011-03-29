/**\file ntbp_membrane_compartment_obj.cpp - NTBP_membrane_compartment_o class implementation
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


/* $Id: ntbp_membrane_compartment_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
* $Log: ntbp_membrane_compartment_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.6  2003/06/20 16:42:53  face
* *** empty log message ***
*
* Revision 1.4  2002/03/13 19:23:27  face
* *** empty log message ***
*
* Revision 1.3  2002/02/24 20:56:04  face
* *** empty log message ***
*
* Revision 1.2  2002/01/21 17:24:28  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#include "ntbp_membrane_compartment_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_membrane_compartment_o */
NTBP_membrane_compartment_o::NTBP_membrane_compartment_o(NTreal newArea /* in muMeter^2 */, NTreal newTemperature)
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
NTBP_membrane_compartment_o::NTBP_membrane_compartment_o(const NTBP_membrane_compartment_o & original)
{
 // should not be used
	cerr << "NTBP_membrane_compartment_o::NTBP_membrane_compartment_o(const .. & original)= - ERROR : Not implemented." << endl;
}

const NTBP_membrane_compartment_o&
NTBP_membrane_compartment_o::operator= (const NTBP_membrane_compartment_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // should not be used
 cerr << "NTBP_membrane_compartment_o::operator= - ERROR : Not implemented." << endl;
  return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_membrane_compartment_o::~NTBP_membrane_compartment_o()
{
}

/* ***  PUBLIC                                    ***   */
/** @short      The membrane compartment has one modes of operation.
							If the voltage is specified externally at each time step,
								 the compartment acts as a container for the occuring current
								 objects, integratios has to be provided externally.
								 e.g.
   						        NTBP_membrane_compartment_o c; [...]
   						        FLOAT voltage;
								      LOOP
											 voltage = IntegrateDifferentialEquation( c._vM() );
											 c.Step( voltage );
									    END LOOP;
							Additionally single compartment HodgkinHuxley simulation is possible using:
								 where the voltage is updated at each step, if the
								 old voltage is specified as input:
								 e.g. LOOP
								         NTBP_membrane_compartment_o c; [...]
								         c.Step( c._vM() );	deltaVM = 1.0e-3 / sec /mSec / * CompartmentMembraneNetCurrent() / nA / /CompartmentMembraneCapacitance() / muF /;
  											vM += deltaVM * _timeStep();			
							      END LOOP;
    @param      none
    @return     none
   \warning    Voltage is updated at Step
   \bug        unknown
*/
NTreturn
NTBP_membrane_compartment_o::Step(NTreal newVM)
{
	vM = newVM;
	
	//for every current
	for (NTsize it = 0; it < currentVec.size(); it++) {
		(currentVec[it])->Step(vM);
	}
	
	return NT_SUCCESS;
}


/** @short
    @param      none
    @return     none
   \warning    OBSOLETE
   \bug        unknown
*/
NTreal
NTBP_membrane_compartment_o::TotalConductance() const
{
	NTreal result = 0.0;
	vector< NTBP_membrane_current_o * >::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		result += (*it)->_conductance();
	}
	return result;
}

/** @short Conductance weighted with reversal potential
    @param      none
    @return     none
   \warning    OBSOLETE ?
   \bug        unknown
*/
NTreal
NTBP_membrane_compartment_o::WeightedConductance() const
{
	NTreal result = 0.0;
	
	vector< NTBP_membrane_current_o * >::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		result += ( (*it)->_conductance() ) * ( (*it)->_reversalPotential() );
	}
	return result;
}

/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
*/
NTreturn
NTBP_membrane_compartment_o::AttachCurrent(NTBP_membrane_current_o * currentPtr, NTBPcurrentType type = NTBP_IONIC)
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
			cerr <<"NTBP_membrane_compartment_o::AttachCurrent - Error : Unsupported current type "<< type <<"specified." << endl;
			return NT_PARAM_UNSUPPORTED;
	}

	return NT_SUCCESS;
}

NTreturn
NTBP_membrane_compartment_o::InjectCurrent(NTreal current /* in nA */)
{
//	NT_ASSERT(current >=0 ); 2DO is this necessary
	iInj = current;
	return NT_SUCCESS;	
}

/** The total membrane capacitance of the compartment
	@return muF
*/
NTreal NTBP_membrane_compartment_o::CompartmentMembraneCapacitance() const
{
	return (cM /* muF/cm^2 */ * area /* muMeter^2 */ * 1.0e-8 /* cm^2/muMeter^2 */);
}

/** The net flowing current through the membrane of the compartment
	@return ?
*/
NTreal NTBP_membrane_compartment_o::CompartmentMembraneNetCurrent() const
{
	NTreal sumDeltaI = 0.0;
	for (NTsize it = 0; it < currentVec.size(); it++) {
			sumDeltaI -= (currentVec[it])->_current();	// i.e. ionic current is subtracted (modern  current convention)
	}
//	cerr << "\t Membrane capacitance [muF]"<< CompartmentMembraneCapacitance() << "\t Injected current [nA] "<< iInj <<  "\tionicCurrent="<<sumDeltaI << endl;	
	sumDeltaI += iInj; // the is  sign correct
	return sumDeltaI;
}

/** Sum of escape rates from current state [1/kHz] */
NTreal NTBP_membrane_compartment_o::CompartmentChannelStateTimeConstant() const
{
  cerr <<"NTBP_membrane_compartment_o::CompartmentChannelStateTimeConstant" << endl;
	NTreal sum = 0;
	vector< NTBP_membrane_current_o * >::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
			//(*it)->ComputeRateConstants(_vM());
			sum += (*it)->ChannelStateTimeConstant();
			cerr << "Compartment Time constant sum=" << sum << endl;
	}


	return sum;
}


bool NTBP_membrane_compartment_o::GillespieStep()
{
	cerr << "NTBP_membrane_compartment_o::GillespieStep()" << endl;
	NT_uniform_rnd_dist_o rnd;
	NTreal val = rnd.RndVal();
	NTreal sum = 0.0;		
	
  // 2DO this might be actually called more then once in one total iteration time step
	NTreal	compartmentTau = CompartmentChannelStateTimeConstant();
	
	cerr << "COMPARTMENT -> compartmentTau=" << compartmentTau << endl;
	vector< NTBP_membrane_current_o * >::iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
				sum += (*it)->ChannelStateTimeConstant();
				cerr << "COMPARTMENT -> SUM=" << sum  << " VAL=" << val <<  endl;
				if (val < sum/compartmentTau){
					return (*it)->GillespieStep();
					break;
				}
	}
	cerr <<"NTBP_membrane_compartment_o::GillespieStep - Error : Control flow should not reach here." << endl;
	return NT_FAIL;
}

void NTBP_membrane_compartment_o::ShowParam() const
{
	cout << "Compartment paramters:" << endl;
	cout << "Specific membrane capacitance [muF/cm^2] " << _cM() << endl;
	cout << "Axoplasmic resitance [Ohm cm] " << _rA() << endl;
	cout << "Membrane surface area [muMeter^2] " << _area() << endl;
	cout << "Compartment currents paramters:" << endl;
	vector< NTBP_membrane_current_o * >::const_iterator it = currentVec.begin();
	for (it = currentVec.begin(); it != currentVec.end(); it++) {
		(*it)->ShowParam();	
	}
}

vector< NTBP_membrane_current_o * > NTBP_membrane_compartment_o::ReturnCurrentVec ()
{
	vector< NTBP_membrane_current_o * > cV = currentVec;
	return cV;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
