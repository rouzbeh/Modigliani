///**\file ntbp_hranvier_potassium_multi_current_obj.cpp - squid giant axon sodium conducta nce class implementation
// * by Ahmed Aldo Faisal &copy; created 16.3.2001
// */
///* NetTrader - visualisation, scientific and financial analysis and simulation system
// * Version:  0.5
// * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
// *
// * This library is free software; you can redistribute it and/or
// * modify it under the terms of the GNU Library General Public
// * License as published by the Free Software Foundation; either
// * version 2 of the License, or (at your option) any later version.
// *
// * This library is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// * Library General Public License for more details.
// *
// * You should have received a copy of the GNU Library General Public
// * License along with this library; if not, write to the Free
// * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// */
//
///* $Id: ntbp_hranvier_potassium_multi_current_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
// * $Log: ntbp_hranvier_potassium_multi_current_obj.cpp,v $
// * Revision 1.1.1.1  2004/12/16 01:38:36  face
// * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
// *
// * Revision 1.1  2003/01/30 17:13:42  face
// * *** empty log message ***
// *
// * Revision 1.4  2002/03/13 19:23:27  face
// * *** empty log message ***
// *
// * Revision 1.3  2001/11/06 16:28:13  face
// * *** empty log message ***
// *
// * Revision 1.2  2001/10/12 09:18:36  face
// * *** empty log message ***
// *
// * Revision 1.1  2001/10/03 14:37:52  face
// * *** empty log message ***
// *
//
// */
//#include "ntbp_hranvier_potassium_multi_current_obj.h"
//
//NT_gaussian_rnd_dist_o NTBP_hranvier_potassium_multi_current_o::normalRnd;
//bool NTBP_hranvier_potassium_multi_current_o::initTableLookUp = false;
//NTreal NTBP_hranvier_potassium_multi_current_o::alphaNvec[15000];
//NTreal NTBP_hranvier_potassium_multi_current_o::betaNvec[15000];
//
///* ***      CONSTRUCTORS	***/
///** Create a NTBP_hranvier_potassium_multi_current_o */
//NTBP_hranvier_potassium_multi_current_o::NTBP_hranvier_potassium_multi_current_o(
//		NTreal newArea, NTreal newDensity, NTreal newChannelConductance,
//		NTreal newVBase, NTreal newQ10, NTreal reversalPotential) :
//	q10(newQ10), NTBP_multi_current_o(reversalPotential /* mV */, newDensity,
//			newArea, newChannelConductance, newVBase) {
//	NT_ASSERT (newArea >= 0);
//	NT_ASSERT (newDensity >= 0);
//	NT_ASSERT (newChannelConductance >= 0);
//	UpdateNumChannels();
//	baseTemp = 20.0; // C
//
//	NTreal vTmp = 0;
//	if (false == initTableLookUp) {
//		for (NTsize ll = 0; ll < 15000; ll++) {
//			vTmp = ((float) ll) / 100.0 - 20.0 + 0.005/* otherwise we hit undefined points of alpha and beta function */;
//			alphaNvec[ll] = AlphaN(vTmp);
//			betaNvec[ll] = BetaN(vTmp);
//		}
//		initTableLookUp = true;
//	}
//	n = alphaN / (alphaN + betaN);
//
//	noiseN = 0.0;
//
//	channelsPtr = new NTBP_ion_channels_o(_numChannels(), 5, newTimeStep);
//	channelsPtr->setAsOpenState(5);
//
//	setTimeStep( newTimeStep);
//	Set_temperature( newTemperature);
//
//}
//
///* ***      COPY AND ASSIGNMENT	***/
//NTBP_hranvier_potassium_multi_current_o::NTBP_hranvier_potassium_multi_current_o(
//		const NTBP_hranvier_potassium_multi_current_o & original) :
//	NTBP_multi_current_o(original._reversalPotential(), original._density(),
//			original._area(), original._conductivity()) {
//	// add assignment code here
//	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 5, original._timeStep());
//	channelsPtr->setAsOpenState(5);
//}
//
//const NTBP_hranvier_potassium_multi_current_o&
//NTBP_hranvier_potassium_multi_current_o::operator=(
//		const NTBP_hranvier_potassium_multi_current_o & right) {
//	if (this == &right)
//		return *this; // Gracefully handle self assignment
//	channelsPtr = new NTBP_potassium_ion_channels_o(right._numChannels());
//	channelsPtr->setAsOpenState(5);
//	return *this;
//}
//
///* ***      DESTRUCTOR		***/
//NTBP_hranvier_potassium_multi_current_o::~NTBP_hranvier_potassium_multi_current_o() {
//	delete channelsPtr;
//}
//
///* ***  PUBLIC                                    ***   */
///** @short
// @param      none
// @return     none
// \warning    unknown
// \bug        unknown
// */
//
//inline void NTBP_hranvier_potassium_multi_current_o::ComputeRateConstants() {
//	NTreal q10Factor = NTBP_TemperatureRateRelation(_temperature(),
//			baseTemp /* C */, q10);
//
//	NTsize index = 0;
//	NTreal vM = -100;
//	NTreal deltaT = _timeStep();
//
//	for (NTsize i = 0; i < 5000; i++) {
//		vM += 0.1;
//		if ((vM < -20.0) || (vM > 130.0)) {
//			alphaN = q10Factor * AlphaN(vM);
//			betaN = q10Factor * BetaN(vM);
//		} else {
//			index = (NTsize) floor((vM + 20.0) * 100.0);
//			alphaN = q10Factor * alphaNvec[index];
//			betaN = q10Factor * betaNvec[index];
//		}
//
//		NTreal alphaNdeltaT = alphaN * deltaT;
//		NTreal betaNdeltaT = betaN * deltaT;
//
//		channelsPtr->setTransactionProbability(i, 1, 2, 4 * alphaNdeltaT);
//		channelsPtr->setTransactionProbability(i, 2, 3, 3 * alphaNdeltaT);
//		channelsPtr->setTransactionProbability(i, 3, 4, 2 * alphaNdeltaT);
//		channelsPtr->setTransactionProbability(i, 4, 5, 1 * alphaNdeltaT);
//		channelsPtr->setTransactionProbability(i, 5, 4, 4 * betaNdeltaT);
//		channelsPtr->setTransactionProbability(i, 4, 3, 3 * betaNdeltaT);
//		channelsPtr->setTransactionProbability(i, 3, 2, 2 * betaNdeltaT);
//		channelsPtr->setTransactionProbability(i, 2, 1, 1 * betaNdeltaT);
//	}
//	channelsPtr->setRatesComputed(true);
//}
//
///* ***  PROTECTED                         ***   */
///*NTreturn DEPRECATED
// NTBP_hranvier_potassium_multi_current_o::SetDensityChild(NTreal channelDensity)// in channels/muMeter^2
// {
// // 2DO improve : copy the state Besetzungszustaende
// density = channelDensity;
// delete	channelsPtr;
// channelsPtr = new NTBP_potassium_ion_channels_o( density*area );
// return NT_SUCCESS;
// }
// */
//
///** in kHz */
//inline NTreal NTBP_hranvier_potassium_multi_current_o::ComputeChannelStateTimeConstant() const {
//	return channelsPtr->ComputeChannelStateTimeConstant(voltage);
//}
//
///** No descriptions */
//inline NTreal NTBP_hranvier_potassium_multi_current_o::OpenChannels() const {
//	switch (_simulationMode()) {
//	case NTBP_BINOMIALPOPULATION:
//	case NTBP_GILLESPIE:
//	case NTBP_SINGLECHANNEL:
//	case NTBP_DETERMINISTIC:
//		return channelsPtr->NumOpen();
//		break;
//	case NTBP_LANGEVIN:
//		return n * n * n * n * NumChannels();
//	default:
//		cerr
//				<< "NTBP_hranvier_potassium_multi_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels()."
//				<< endl;
//		return 0;
//	}
//}
//
///** No descriptions */
//inline NTreal NTBP_hranvier_potassium_multi_current_o::OpenChannelsRatio() const {
//	return 100 * OpenChannels() / NumChannels();
//}
//
//inline NTreal NTBP_hranvier_potassium_multi_current_o::ComputeConductance() {
//	switch (_simulationMode()) {
//	case NTBP_GILLESPIE:
//	case NTBP_SINGLECHANNEL:
//	case NTBP_BINOMIALPOPULATION:
//	case NTBP_DETERMINISTIC:
//		return Set_conductance(channelsPtr->NumOpen() * conductivity);
//		break;
//	case NTBP_LANGEVIN:
//		return Set_conductance(_maxConductivity() /* mS/cm^2 */* n * n * n * n
//				* _area()
//		/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
//		break;
//	case NTBP_NOISYMEAN: {
//		cerr << "NOISYMEAN: NOT IMPLEMENTED PROPERLY" << endl;
//		NTreal mean = n * n * n * n;
//		NTreal numAddOpening = n * n * n * (1 - n) * alphaN / _timeStep();
//		NTreal numAddClosing = n * n * n * n * 4.0 * betaN / _timeStep();
//		//					mean = uniformRnd.RndVal()
//		return Set_conductance(_maxConductivity() /* mS/cm^2 */* mean * _area()
//		/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
//	}
//	default:
//		cerr
//				<< "NTBP_hranvier_potassium_multi_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance."
//				<< endl;
//		return 0;
//	}
//}
//
//inline NTreturn NTBP_hranvier_potassium_multi_current_o::StepCurrent() {
//	vector<NTreal> vec(2);
//	vec[0] = alphaN;
//	vec[1] = betaN;
//
//	NTsize counter = 0;
//	NTreal tmpN = 0;
//
//	switch (_simulationMode()) {
//	case NTBP_BINOMIALPOPULATION:
//		//NT_ASSERT(	((NTBP_potassium_ion_channels_o*) channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
//		return (channelsPtr->BinomialStep(voltage));
//		break;
//	case NTBP_GILLESPIE:
//		//return ((NTBP_potassium_ion_channels_o*)channelsPtr)->UpdateStateProb(vec);
//		return NT_SUCCESS;
//		break;
//	case NTBP_SINGLECHANNEL:
//		//NT_ASSERT(	((NTBP_potassium_ion_channels_o*)channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
//		return (channelsPtr->Step(voltage));
//		break;
//	case NTBP_LANGEVIN:
//		counter = 0;
//		n += _timeStep() * ((1.0 - n) * alphaN - n * betaN);
//		NT_ASSERT(n>=0 && n<= 1);
//		do {
//			counter++;
//			tmpN = _timeStep() * normalRnd.RndVal() * sqrt((alphaN * (1 - n)
//					+ betaN * n) / _numChannels());
//			if (counter > 1 && counter <= 1024)
//				cerr << "Kn=" << counter << endl;
//			else if (counter > 1024) {
//				noiseN = 0;
//				tmpN = 0;
//				break;
//			}
//		} while ((_timeStep() * (tmpN + noiseN) + n >= 1) || (_timeStep()
//				* (tmpN + noiseN) + n <= 0));
//		noiseN += tmpN;
//		n += _timeStep() * noiseN;
//		return NT_SUCCESS;
//		break;
//	case NTBP_NOISYMEAN:
//	case NTBP_DETERMINISTIC:
//		n += _timeStep() * ((1.0 - n) * alphaN - n * betaN);
//		NT_ASSERT(n>=0 && n<= 1);
//		return NT_SUCCESS;
//		break;
//	default:
//		cerr
//				<< "NTBP_hranvier_potassium_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode."
//				<< endl;
//		return NT_PARAM_UNSUPPORTED;
//	}
//}
//
//void NTBP_hranvier_potassium_multi_current_o::ShowParam() const {
//	cout << "K channel parameters:" << endl;
//	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
//	cout << "Channel density [1/muMeter^2]" << _area() << endl;
//	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
//			<< _maxConductivity() << endl;
//}
//
///* ***  PRIVATE                           ***   */
//
///* File skeleton generated by GenNTObj version 0.7. */
///**  */
///**  */
