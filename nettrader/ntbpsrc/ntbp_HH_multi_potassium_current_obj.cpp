/**\file ntbp_HH_multi_potassium_current_obj.cpp - squid giant axon sodium conducta nce class implementation
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

/* $Id: ntbp_HH_multi_potassium_current_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: ntbp_HH_multi_potassium_current_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.5  2003/01/30 17:13:44  face
 * *** empty log message ***
 *
 * Revision 1.4  2002/03/13 19:23:27  face
 * *** empty log message ***
 *
 * Revision 1.3  2001/11/06 16:28:13  face
 * *** empty log message ***
 *
 * Revision 1.2  2001/10/12 09:18:36  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#include "ntbp_HH_multi_potassium_current_obj.h"

NT_gaussian_rnd_dist_o NTBP_HH_multi_potassium_current_o::normalRnd;
bool NTBP_HH_multi_potassium_current_o::initTableLookUp = false;
NTreal NTBP_HH_multi_potassium_current_o::alphaNvec[15000];
NTreal NTBP_HH_multi_potassium_current_o::betaNvec[15000];

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_HH_multi_potassium_current_o */
NTBP_HH_multi_potassium_current_o::NTBP_HH_multi_potassium_current_o(
		NTreal newArea, NTreal newDensity, NTreal newConductivity,
		NTreal newVBase, NTreal newQ10n,
		NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature) :
	q10n(newQ10n), NTBP_multi_current_o(
			reversalPotential /* in mV */, newDensity /* channels per mu^2 */,
			newArea /* in mu^2 */, newConductivity /* in mS per channel  */,
			newVBase /* mV */
	) {
	//density and area updated by NTBP_multi_current_obj constructor
	UpdateNumChannels();
	channelsPtr = new NTBP_ion_channels_o(_numChannels(), 5, newTimeStep);
	channelsPtr->setAsOpenState(5);
	//TODO: What are noiseM / -H?
	noiseN = 0;
	baseTemp = 6.3;
	setTimeStep(newTimeStep);
	Set_temperature(newTemperature);
	NTreal vTmp = 0;

	if (false == initTableLookUp) {
		for (NTsize ll = 0; ll < 15000; ll++) {
			vTmp = ((float) ll) / 100.0 - 20 + 0.005 /* otherwise we hit undefined points of alpha and beta function */;
			alphaNvec[ll] = AlphaN(vTmp);
			betaNvec[ll] = BetaN(vTmp);
		}
		initTableLookUp = true;
	}

	// this is n_inf
	n = AlphaN(0) / (AlphaN(0) + BetaN(0));
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_HH_multi_potassium_current_o::NTBP_HH_multi_potassium_current_o(
		const NTBP_HH_multi_potassium_current_o & original) :
	q10n(original.q10n), NTBP_multi_current_o(
			original._reversalPotential(), original._density(),
			original._area(), original._conductivity()) {
	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 5);
	channelsPtr->setAsOpenState(5);
}

const NTBP_HH_multi_potassium_current_o&
NTBP_HH_multi_potassium_current_o::operator=(
		const NTBP_HH_multi_potassium_current_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 5);
	channelsPtr->setAsOpenState(5);
	return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_HH_multi_potassium_current_o::~NTBP_HH_multi_potassium_current_o() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline NTreturn NTBP_HH_multi_potassium_current_o::StepCurrent() {
	//	cerr << "NTBP_HH_multi_potassium_current_o::StepCurrent()" << endl;
	NTreal tmpN = 0;
	NTsize counter = 0;
	if (!channelsPtr->getRatesComputed()) {
		ComputeRateConstants();
	}

	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION: {
		return (channelsPtr->BinomialStep(voltage));
	}
		break;
	case NTBP_GILLESPIE: {
		cerr << "WARNING : NTBP_GILLESPIE is being called on multi potassium channel. NOT IMPLEMENTED.";
		return NT_NOT_IMPLEMENTED;
	}
		break;
	case NTBP_SINGLECHANNEL: {
		return (channelsPtr->Step(voltage));
	}
		break;
	case NTBP_LANGEVIN: {
		/*counter = 0;
		m += _timeStep() * ((1.0 - m) * alphaM - m * betaM);
		NT_ASSERT(m>=0 && m<=1);
		do {
			counter++;
			tmpM = _timeStep() * normalRnd.RndVal() * sqrt((alphaM * (1 - m)
					+ betaM * m) / _numChannels());
			if (counter > 1 && counter < 1024)
				cerr << "NaM=" << counter << endl;
			else if (counter >= 1024) {
				noiseM = 0;
				tmpM = 0;
				break;
			}
		} while ((_timeStep() * (tmpM + noiseM) + m >= 1) || (_timeStep()
				* (tmpM + noiseM) + m <= 0));
		noiseM += tmpM;
		m += _timeStep() * noiseM;

		counter = 0;
		h += _timeStep() * ((1.0 - h) * alphaH - h * betaH);
		NT_ASSERT(h>=0 && h<= 1);
		do {
			counter++;
			tmpH = _timeStep() * normalRnd.RndVal() * sqrt((alphaH * (1 - h)
					+ betaH * h) / _numChannels());
			if (counter > 1 && counter < 1024)
				cerr << "NaH=" << counter << endl;
			else if (counter >= 1024) {
				noiseH = 0;
				tmpH = 0;
				break;
			}
		} while ((_timeStep() * (tmpH + noiseH) + h >= 1) || (_timeStep()
				* (tmpH + noiseH) + h <= 0));
		noiseH += tmpH;
		h += _timeStep() * noiseH;

		return NT_SUCCESS;*/
		cerr << "WARNING : NTBP_LANGEVIN is being called on multi potassium channel. NOT IMPLEMENTED.";
		return NT_NOT_IMPLEMENTED;
	}
		break;
	case NTBP_NOISYMEAN:
	case NTBP_DETERMINISTIC:
		n += _timeStep() * ((1.0 - n) * AlphaN(voltage) - n * BetaN(voltage)); //TODO: multiply by Q10s?
		//if(voltage>-2.33 && voltage < -2.32) cerr << "V: " << voltage << "\tn:" << n << endl;
		NT_ASSERT(n>=0 && n<= 1);
		return NT_SUCCESS;
		break;
	default:
		cerr
				<< "NTBP_HH_multi_potassium_current_o::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return NT_PARAM_UNSUPPORTED;
		break;
	}
	return NT_FAIL;
}

inline void NTBP_HH_multi_potassium_current_o::ComputeRateConstants() {
	cerr << "Calculating rate matrix for NTBP_HH_multi_potassium_current_o" << endl;
	NTreal temp = _temperature();
	NTreal deltaT = _timeStep();
	NTreal q10FactorN = NTBP_TemperatureRateRelation(temp, baseTemp /* C */,q10n);
//	cerr  << "q10 = " << q10FactorN << endl;
	NTsize index = 0;
	NTreal vM = -100;

	for (NTsize i = 0; i < 5000; i++) {
		vM += 0.1;
		NTreal alphaN, betaN;
		if ((vM < -20) || (vM > 130.0)) {
			alphaN = q10FactorN * AlphaN(vM);
			betaN = q10FactorN * BetaN(vM);
		} else {
			index = (NTsize) floor((vM + 20) * 100.0);
			alphaN = q10FactorN * alphaNvec[index];
			betaN = q10FactorN * betaNvec[index];
		}

		NTreal alphaNdeltaT = alphaN * deltaT;
		NTreal betaNdeltaT = betaN * deltaT;

		channelsPtr->setTransactionProbability(i, 1, 2, 4 * alphaNdeltaT);
		channelsPtr->setTransactionProbability(i, 2, 3, 3 * alphaNdeltaT);
		channelsPtr->setTransactionProbability(i, 3, 4, 2 * alphaNdeltaT);
		channelsPtr->setTransactionProbability(i, 4, 5, 1 * alphaNdeltaT);

		channelsPtr->setTransactionProbability(i, 5, 4, 4 * betaNdeltaT);
		channelsPtr->setTransactionProbability(i, 4, 3, 3 * betaNdeltaT);
		channelsPtr->setTransactionProbability(i, 3, 2, 2 * betaNdeltaT);
		channelsPtr->setTransactionProbability(i, 2, 1, 1 * betaNdeltaT);


	}

	channelsPtr->setRatesComputed(true);
}

/**  */
/** No descriptions */
inline NTreal NTBP_HH_multi_potassium_current_o::OpenChannels() const {
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
		return channelsPtr->NumOpen();
		break;
	case NTBP_LANGEVIN:
	case NTBP_DETERMINISTIC:
		return pow(n,4) * NumChannels();
	default:
		cerr
				<< "NTBP_HH_multi_potassium_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels."
				<< endl;
		return 0;
	}
}

/**  */
/** No descriptions */
inline NTreal NTBP_HH_multi_potassium_current_o::OpenChannelsRatio() const {
	return OpenChannels() * 100 / NumChannels();
}

inline NTreal NTBP_HH_multi_potassium_current_o::ComputeConductance() {
	//  cerr << "NTBP_HH_multi_potassium_current_o::ComputeConductance" << endl;
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
		return Set_conductance(channelsPtr->NumOpen() * conductivity);
		break;
	case NTBP_LANGEVIN:
	case NTBP_DETERMINISTIC:

//		cerr << Set_conductance(_maxConductivity() /* mS/cm^2 */* pow(n,4) * _area() /* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */) << endl;
		return Set_conductance(_maxConductivity() /* mS/cm^2 */* pow(n,4) * _area()/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	case NTBP_NOISYMEAN: {
		/*NTreal mean = n^4;
		NTreal numAddOpening = m * m * (1 - m) * (1 - h) * alphaM / _timeStep()
				+ m * m * m * (1 - h) * alphaH / _timeStep();
		NTreal numAddClosing = m * m * m * h * (3 * betaM + betaH)
				/ _timeStep();
		mean += numAddOpening - numAddClosing;*/
		cerr << "NTBP_HH_multi_potassium_current_o::ComputeConductance(NTBP_NOISYMEAN): NOT IMPLEMENTED CORRECTLY" << endl;
		//return Set_conductance(_maxConductivity() /* mS/cm^2 */* mean * _area()
		return NT_NOT_IMPLEMENTED;
		// /* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	}
	default:
		cerr
				<< "NTBP_HH_multi_potassium_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance."
				<< endl;
		return 0;
	}
}

inline NTreal NTBP_HH_multi_potassium_current_o::ComputeChannelStateTimeConstant() const {
	// cerr << "NTBP_HH_multi_potassium_current_o::ComputeChannelStateTimeConstant()" << endl;
	return channelsPtr->ComputeChannelStateTimeConstant(voltage);
}

void NTBP_HH_multi_potassium_current_o::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}
