/**\file ntbp_multi_sodium_current_obj.cpp - squid giant axon sodium conducta nce class implementation 
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

/* $Id: ntbp_multi_sodium_current_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_multi_sodium_current_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.9  2003/07/21 13:30:57  face
 * *** empty log message ***
 *
 * Revision 1.8  2003/01/30 17:13:44  face
 * *** empty log message ***
 *
 * Revision 1.7  2002/12/12 15:31:45  face
 * *** empty log message ***
 *
 * Revision 1.6  2002/03/13 19:23:27  face
 * *** empty log message ***
 *
 * Revision 1.5  2002/03/08 15:13:26  face
 * *** empty log message ***
 *
 * Revision 1.4  2002/01/21 17:24:28  face
 * *** empty log message ***
 *
 * Revision 1.3  2001/11/06 16:28:13  face
 * *** empty log message ***
 *
 * Revision 1.2  2001/10/12 09:18:36  face
 * *** empty log message ***
 *

 */
#include "ntbp_multi_sodium_current_obj.h"
NT_gaussian_rnd_dist_o NTBP_multi_sodium_current_o::normalRnd;
bool NTBP_multi_sodium_current_o::initTableLookUp = false;
NTreal NTBP_multi_sodium_current_o::alphaMvec[15000];
NTreal NTBP_multi_sodium_current_o::betaMvec[15000];
NTreal NTBP_multi_sodium_current_o::alphaHvec[15000];
NTreal NTBP_multi_sodium_current_o::betaHvec[15000];

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_multi_sodium_current_o */
NTBP_multi_sodium_current_o::NTBP_multi_sodium_current_o(NTreal newArea,
		NTreal newDensity, NTreal newConductivity, NTreal newVBase,
		NTreal newReversalPotential) :
			NTBP_multi_current_o(newReversalPotential /* in mV */,
					newDensity /* channels per mu^2 */, newArea /* in mu^2 */,
					newConductivity /* in mS per channel  */, newVBase) {
	UpdateNumChannels();
	channelsPtr = new NTBP_ion_channels_o(_numChannels(), 8);
	noiseM = 0;
	noiseH = 0;
	NTreal vTmp = 0;

	if (false == initTableLookUp) {
		for (NTsize ll = 0; ll < 15000; ll++) {
			vTmp = ((float) ll) / 100.0 - 20.0 + 0.005 /* otherwise we hit undefined points of alpha and beta function */;
			alphaMvec[ll] = AlphaM(vTmp);
			betaMvec[ll] = BetaM(vTmp);
			alphaHvec[ll] = AlphaH(vTmp);
			betaHvec[ll] = BetaH(vTmp);
		}
		initTableLookUp = true;
	}

	ComputeRateConstants();
	m = alphaM / (alphaM + betaM);
	h = alphaH / (alphaH + betaH);

	channelsPtr->setAsOpenState(4);

	//channelsPtr->SteadyStateDistribution();
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_multi_sodium_current_o::NTBP_multi_sodium_current_o(
		const NTBP_multi_sodium_current_o & original) :
			NTBP_multi_current_o(original._reversalPotential(),
					original._density(), original._area(),
					original._conductivity()) {
	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 8);
	channelsPtr->setAsOpenState(4);
	ComputeRateConstants();

}

const NTBP_multi_sodium_current_o&
NTBP_multi_sodium_current_o::operator=(
		const NTBP_multi_sodium_current_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 8);
	channelsPtr->setAsOpenState(4);
	ComputeRateConstants();
	return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_multi_sodium_current_o::~NTBP_multi_sodium_current_o() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline NTreturn NTBP_multi_sodium_current_o::StepCurrent() {
	//	cerr << "NTBP_multi_sodium_current_o::StepCurrent()" << endl;
	NTreal tmpM = 0;
	NTreal tmpH = 0;
	NTsize counter = 0;

	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION: {
		//		vector<NTreal> vec(4);
		//		vec[0] = alphaM;
		//		vec[1] = betaM;
		//		vec[2] = alphaH;
		//		vec[3] = betaH;
		//NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
		return (channelsPtr->BinomialStep(voltage));
	}
		break;
	case NTBP_GILLESPIE: {
		//		vector<NTreal> vec(4);
		//		vec[0] = alphaM;
		//		vec[1] = betaM;
		//		vec[2] = alphaH;
		//		vec[3] = betaH;
		//return (channelsPtr)->UpdateStateProb(vec);
		return NT_SUCCESS;
	}
		break;
	case NTBP_SINGLECHANNEL: {
		//		vector<NTreal> vec(4);
		//		vec[0] = alphaM;
		//		vec[1] = betaM;
		//		vec[2] = alphaH;
		//		vec[3] = betaH;
		//NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
		return (channelsPtr->Step(voltage));
	}
		break;
	case NTBP_LANGEVIN: {
		counter = 0;
		m += _timeStep() * ((1.0 - m) * alphaM - m * betaM);
		NT_ASSERT(m>=0 && m<=1);
		do {
			counter++;
			tmpM = _timeStep() * normalRnd.RndVal() * sqrt(
					(alphaM * (1 - m) + betaM * m) / _numChannels());
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
			tmpH = _timeStep() * normalRnd.RndVal() * sqrt(
					(alphaH * (1 - h) + betaH * h) / _numChannels());
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

		return NT_SUCCESS;
	}
		break;
	case NTBP_NOISYMEAN:
	case NTBP_DETERMINISTIC:
		m += _timeStep() * ((1.0 - m) * alphaM - m * betaM);
		NT_ASSERT(m>=0 && m<= 1);
		h += _timeStep() * ((1.0 - h) * alphaH - h * betaH);
		NT_ASSERT(h>=0 && h<= 1);
		return NT_SUCCESS;
		break;
	default:
		cerr
				<< "NTBP_multi_sodium_current_o::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return NT_PARAM_UNSUPPORTED;
		break;
	}
	return NT_FAIL;
}

inline void NTBP_multi_sodium_current_o::ComputeRateConstants() {
	NTreal vM = -100;
	NTreal temp = _temperature();
	NTreal deltaT = _timeStep();
	NTreal q10FactorM = NTBP_TemperatureRateRelation(_temperature(), _q10());
	NTreal q10FactorH = NTBP_TemperatureRateRelation(_temperature(), _q10());
	for (NTsize i = 0; i < 3000; i++) {
		vM += 0.1;

		alphaM = q10FactorM * AlphaM(vM);
		betaM = q10FactorM * BetaM(vM);

		alphaH = q10FactorH * AlphaH(vM);
		betaH = q10FactorH * BetaH(vM);

		NTreal alphaMdeltaT = alphaM * deltaT;
		NTreal betaMdeltaT = betaM * deltaT;
		NTreal alphaHdeltaT = alphaH * deltaT;
		NTreal betaHdeltaT = betaH * deltaT;

		channelsPtr->setTransactionProbability(i, 1, 2, 3 * alphaMdeltaT);
		channelsPtr->setTransactionProbability(i, 2, 3, 2 * alphaMdeltaT);
		channelsPtr->setTransactionProbability(i, 3, 4, alphaMdeltaT);
		channelsPtr->setTransactionProbability(i, 5, 6,
				channelsPtr->getTransactionProbability(i, 1, 2));
		channelsPtr->setTransactionProbability(i, 6, 7,
				channelsPtr->getTransactionProbability(i, 2, 3));
		channelsPtr->setTransactionProbability(i, 7, 8,
				channelsPtr->getTransactionProbability(i, 3, 4));

		channelsPtr->setTransactionProbability(i, 8, 7, 3 * betaMdeltaT);
		channelsPtr->setTransactionProbability(i, 7, 6, 2 * betaMdeltaT);
		channelsPtr->setTransactionProbability(i, 6, 5, betaMdeltaT);
		channelsPtr->setTransactionProbability(i, 4, 3,
				channelsPtr->getTransactionProbability(i, 8, 7));
		channelsPtr->setTransactionProbability(i, 3, 2,
				channelsPtr->getTransactionProbability(i, 7, 6));
		channelsPtr->setTransactionProbability(i, 2, 1,
				channelsPtr->getTransactionProbability(i, 6, 5));

		channelsPtr->setTransactionProbability(i, 1, 5, betaHdeltaT);
		channelsPtr->setTransactionProbability(i, 2, 6, betaHdeltaT);
		channelsPtr->setTransactionProbability(i, 3, 7, betaHdeltaT);
		channelsPtr->setTransactionProbability(i, 4, 8, betaHdeltaT);

		channelsPtr->setTransactionProbability(i, 5, 1, alphaHdeltaT);
		channelsPtr->setTransactionProbability(i, 6, 2, alphaHdeltaT);
		channelsPtr->setTransactionProbability(i, 7, 3, alphaHdeltaT);
		channelsPtr->setTransactionProbability(i, 8, 4, alphaHdeltaT);
	}
}

/** NTBP_multi_sodium_current_o::OpenChannels
 Note, we assume that numChannels is based on true total number of channels
 and not on peak current estimates the count the max open number of channels.
 */
inline NTreal NTBP_multi_sodium_current_o::OpenChannels() const {
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
		return channelsPtr->NumOpen();
		break;
	case NTBP_LANGEVIN:
	case NTBP_DETERMINISTIC:
		return m * m * m * h * NumChannels();
		break;
	default:
		cerr
				<< "NTBP_multi_sodium_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels."
				<< endl;
		return 0;
	}
}

inline NTreal NTBP_multi_sodium_current_o::ComputeConductance() {
	//  cerr << "NTBP_multi_sodium_current_o::ComputeConductance" << endl;
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
		return Set_conductance(channelsPtr->NumOpen() * conductivity);
		break;
	case NTBP_LANGEVIN:
	case NTBP_DETERMINISTIC:
		return Set_conductance(
				_maxConductivity() /* mS/cm^2 */* m * m * m * h * _area()
				/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	case NTBP_NOISYMEAN: {
		NTreal mean = m * m * m * h;
		NTreal numAddOpening = m * m * (1 - m) * (1 - h) * alphaM / _timeStep()
				+ m * m * m * (1 - h) * alphaH / _timeStep();
		NTreal numAddClosing = m * m * m * h * (3 * betaM + betaH)
				/ _timeStep();
		mean += numAddOpening - numAddClosing;
		cerr << "NOT IMPLEMENTED CORRECTLY" << endl;
		return Set_conductance(_maxConductivity() /* mS/cm^2 */* mean * _area()
		/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	}
	default:
		cerr
				<< "NTBP_multi_sodium_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance."
				<< endl;
		return 0;
	}
}

inline NTreal NTBP_multi_sodium_current_o::ComputeChannelStateTimeConstant() const {
	// cerr << "NTBP_multi_sodium_current_o::ComputeChannelStateTimeConstant()" << endl;
	return channelsPtr->ComputeChannelStateTimeConstant(voltage);
}

void NTBP_multi_sodium_current_o::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
