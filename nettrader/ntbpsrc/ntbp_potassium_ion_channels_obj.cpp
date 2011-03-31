///**\file ntbp_potassium_ion_channels_obj.cpp - NTBP_potassium_ion_channels_o class implementation
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
//
///* $Id: ntbp_potassium_ion_channels_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
//* $Log: ntbp_potassium_ion_channels_obj.cpp,v $
//* Revision 1.1.1.1  2004/12/16 01:38:36  face
//* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
//*
//* Revision 1.6  2003/01/30 17:13:44  face
//* *** empty log message ***
//*
//* Revision 1.5  2002/03/13 19:23:27  face
//* *** empty log message ***
//*
//* Revision 1.4  2002/03/08 15:13:26  face
//* *** empty log message ***
//*
//* Revision 1.3  2002/03/04 18:09:07  face
//* added checking for NaN and Inf values in find_spikes_in_binary_file-
//* BinomialStep: redo loop if after state change the states do not sum up
//*
//* Revision 1.2  2001/10/12 09:18:36  face
//* *** empty log message ***
//*
//* Revision 1.1  2001/10/03 14:37:52  face
//* *** empty log message ***
//*
//
//*/
//#include "ntbp_potassium_ion_channels_obj.h"
//
///* ***      CONSTRUCTORS	***/
///** Create a NTBP_potassium_ion_channels_o */
//NTBP_potassium_ion_channels_o::NTBP_potassium_ion_channels_o(NTsize numNewChannels)
//:
//NTBP_ion_channels_o(numNewChannels, 5)
//{
//	for(NTsize ll=0; ll < _numStates()+1; ll++) stateCounterVec[ll] = 0;
//	stateCounterVec[0] = _numChannels();
//	stateCounterVec[1] = _numChannels();
//}
//
///* ***      COPY AND ASSIGNMENT	***/
///*NTBP_potassium_ion_channels_o::NTBP_potassium_ion_channels_o(const NTBP_potassium_ion_channels_o & original)
//{
// // add assignment code here
//}*/
//
//const NTBP_potassium_ion_channels_o&
//NTBP_potassium_ion_channels_o::operator= (const NTBP_potassium_ion_channels_o & right)
//{
// if (this == &right) return *this; // Gracefully handle self assignment
// // add assignment code here
//  return *this;
//}
//
///* ***      DESTRUCTOR		***/
//NTBP_potassium_ion_channels_o::~NTBP_potassium_ion_channels_o()
//{
//}
//
///* ***  PUBLIC                                    ***   */
///** No descriptions */
//inline NTreturn
//NTBP_potassium_ion_channels_o::BinomialStep()
//{
//	vector<NTsize> oldStateCounterVec = stateCounterVec;
//	vector<NTint> newStateCounterVec(stateCounterVec.size());
//	for(NTsize ll=1; ll < _numStates()+1; ll++)	{
//		newStateCounterVec[ll] = stateCounterVec[ll];
//	}
//
//  bool loop = false;
//	NTsize loopCounter = 0;
//
//  do{
//	// ->
//	loop = false;
//	loopCounter++;
//	// ->
//	NTreal delta12 = binomRnd.Binomial(p12, oldStateCounterVec[1]);
//	newStateCounterVec[2] += delta12;
//	newStateCounterVec[1] -= delta12;
//	NTreal delta23 = binomRnd.Binomial(p23, oldStateCounterVec[2]);
//	newStateCounterVec[3] += delta23;
//	newStateCounterVec[2] -= delta23;
//	NTreal delta34 = binomRnd.Binomial(p34,  oldStateCounterVec[3]);
//	newStateCounterVec[4] += delta34;
//	newStateCounterVec[3] -= delta34;
//	NTreal delta45 = binomRnd.Binomial(p45,  oldStateCounterVec[4]);
//	newStateCounterVec[5] += delta45;
//	newStateCounterVec[4] -= delta45;
//
//	NTreal delta21 =  binomRnd.Binomial(p21,  oldStateCounterVec[2]);
//	newStateCounterVec[1] += delta21;
//	newStateCounterVec[2] -= delta21;
//	NTreal delta32 =  binomRnd.Binomial(p32,  oldStateCounterVec[3]);
//	newStateCounterVec[2] += delta32;
//	newStateCounterVec[3] -= delta32;
//	NTreal delta43 =  binomRnd.Binomial(p43,  oldStateCounterVec[4]);
//	newStateCounterVec[3] += delta43;
//	newStateCounterVec[4] -= delta43;
//	NTreal delta54 =  binomRnd.Binomial(p54,  oldStateCounterVec[5]);
//	newStateCounterVec[4] += delta54;
//	newStateCounterVec[5] -= delta54;
//
//
//		/* CHECKING CODE */
//  	NTsize check=0;
//		for(NTsize ll=1; ll < _numStates()+1; ll++)	{
//		check += newStateCounterVec[ll];
//		/** if this bails out here it means that the step sizes (i.e. probabilities) are to large */
//	  if	(newStateCounterVec[ll] < 0 ) loop = true;
//		}
//  	if (check != _numChannels()) loop = true;
//	// after 100 attempts to find an adequate distribution,
//  // ignore any transitions and "skip" this step
//	} while ((true == loop) && (loopCounter < 100));
//
//
//	if (loopCounter < 100) {
//		for(NTsize ll=1; ll < _numStates()+1; ll++)	{
//			stateCounterVec[ll] = newStateCounterVec[ll];
//		}
//	}
////	NT_ASSERT(newStateCounterVec.size() == 9);
////	NT_ASSERT(check == _numChannels());
//	return NT_SUCCESS;
//
//}
//
//
///** Do ot call before rate constants were set. */
//inline NTreturn
//NTBP_potassium_ion_channels_o::SteadyStateDistribution()
//{
////2DO dirty hack ... make time step dependent and maybe compute directly state
////values
//	for (NTsize ll=0; ll< 1000; ll++){
//		BinomialStep();	//much faster then having to wait for Step()
//	}
//	return NT_SUCCESS;
//}
//
//
///** @short
//    @param      none
//    @return     none
//   \warning    unknown
//   \bug        unknown
// */
//inline NTreturn
//NTBP_potassium_ion_channels_o::Step()
//{
//	NTreal rv = 0;
//	vector < NTsize > oldStateCounterVec = stateCounterVec;
//
//	for(NTsize lls=1; lls < _numStates()+1; lls++){
//		for(NTsize llc=0; llc < oldStateCounterVec[lls]; llc++){
//
//		rv = uniformRnd.RndVal();
//
//		switch (lls)
//			{
//			case 1:	if (rv <= p12) {stateCounterVec[2]++;stateCounterVec[1]--;}
//				break;
//			case 2:	 if (rv <= p23) {stateCounterVec[3]++;stateCounterVec[2]--;}
//				else if (rv <= t21) {stateCounterVec[1]++;stateCounterVec[2]--;}
//				break;
//			case 3:  if (rv <= p34) {stateCounterVec[4]++;stateCounterVec[3]--;}
//				else if (rv <= t32) {stateCounterVec[2]++;stateCounterVec[3]--;}
//				break;
//			case 4:  if (rv <= p45) {stateCounterVec[5]++;stateCounterVec[4]--;}
//				else if (rv <= t43) {stateCounterVec[3]++;stateCounterVec[4]--;}
//				break;
//			case 5:  if (rv <= p54) {stateCounterVec[4]++;stateCounterVec[5]--;}
//				break;
//			default:
//				cerr <<"NTBP_potassium_ion_channels_o::Step() - Warning : Invalid state in channel index "<<llc<<" , state "<<lls<<" encountered. State unchanged" << endl;
//				return NT_FAIL;
//				break;
//			}
//		}
//	}
//
//	/* CHECKING CODE */
//	NTsize check=0;
//	for(NTsize ll=1; ll < _numStates()+1; ll++) check += stateCounterVec[ll];
//	NT_ASSERT(check == _numChannels());
//
//	return NT_SUCCESS;
//}
//
//void
//NTBP_potassium_ion_channels_o::ShowStates() const
//{
//	cout << "\t KChannel:";
//	for(NTsize ll=1; ll < _numStates()+1; ll++) cout << stateCounterVec[ll] << " ";
//}
//
///** @short      Compute sum of escape rates (units are [1/s]!)
//    @param      rateConstVec = [ alphaN, betaN ] following standard HH naming convention
//    @return     none
//   \warning    unknown
//   \bug        unknown
//*/
//inline NTreal
//NTBP_potassium_ion_channels_o::ComputeChannelStateTimeConstant()  const
//{
//  cerr <<"NTBP_potassium_ion_channels_o::ComputeChannelStateTimeConstant()" << endl;
//	NTreal sum = 0.0;
//	for (NTsize ll = 0; ll < _numStates(); ll++) {
////		cerr << "K_ION_CHANNELS: statePersistenceProbVec["<<ll<<"]="<<statePersistenceProbVec[ll] << ",stateCounterVec["<<ll+1<<"]="<< stateCounterVec[ll + 1] << endl;
//		sum +=	statePersistenceProbVec[ll] * stateCounterVec[ll + 1];
//	}
//
//	return sum;
//}
//
///**  */
//inline bool
//NTBP_potassium_ion_channels_o::ComputeGillespieStep( NTsize stateId )
//{
//	cerr << "NTBP_potassium_ion_channels_o::ComputeGillespieStep" << endl;
//	NT_uniform_rnd_dist_o rnd;
//	NTreal val = rnd.RndVal()*_timeStep();
//
//	NTsize oldOpen = NumOpen();
////	cerr << "KChannel state to switch "<< stateId << "with 'rate'" << val << "nd state_1_1"	<< p12/statePersistenceProbVec[0] << endl;
////	ShowStates();
//	switch (stateId)
//			{
//			case 1:
//					stateCounterVec[2]++;
//					stateCounterVec[1]--;
//					break;
//			case 2:	 if (val < p23/t21) {stateCounterVec[3]++;stateCounterVec[2]--;}
//				else {stateCounterVec[1]++;stateCounterVec[2]--;}
//				break;
//			case 3:  if (val < p34/t32) {stateCounterVec[4]++;stateCounterVec[3]--;}
//				else {stateCounterVec[2]++;stateCounterVec[3]--;}
//				break;
//			case 4:  if (val < p45/t43) {stateCounterVec[5]++;stateCounterVec[4]--;}
//				else  {stateCounterVec[3]++;stateCounterVec[4]--;}
//				break;
//			case 5:
//					stateCounterVec[4]++;
//					stateCounterVec[5]--;
//				break;
//			default:
//				cerr << "NTBP_potassium_ion_channels_o::ComputeGillespieStep - Error : Invalid stateId specified." << endl;
//				return NT_FAIL;
//	}
//
//	if (NumOpen() == oldOpen) return false;
//	else return true;
//
//
//}
//
//
///** @short
//    @param      rateConstVec = [ alphaN, betaN ] following standard HH naming convention
//    @return     none
//   \warning    unknown
//   \bug        unknown
//*/
//inline NTreturn
//NTBP_potassium_ion_channels_o::UpdateStateProb(vector < NTreal > rateConstVec)
//{
//	NT_ASSERT ( rateConstVec.size() == 2);
//	alphaN = rateConstVec[0];
//	betaN = rateConstVec[1];
//
//	NTreal deltaT = _timeStep();
//	NTreal alphaNdeltaT = alphaN * deltaT;
//	NTreal betaNdeltaT = betaN * deltaT;
//
//	 p12 = 4*alphaNdeltaT;
//	 p23 = 3*alphaNdeltaT;
//	 p34 = 2*alphaNdeltaT;
//	 p45 = alphaNdeltaT;
//
//	 p54 = 4*betaNdeltaT;
//	 p43 = 3*betaNdeltaT;
//	 p32 = 2*betaNdeltaT;
//	 p21 = betaNdeltaT;
//
//	 t21 = p23 + p21;
//	 t32 = p34 + p32;
//	 t43 = p45 + p43;
//
//	// this vector contains the sum of the escape RATES
//	statePersistenceProbVec[0] = 4*alphaN;
//	statePersistenceProbVec[1] = 3*alphaN+betaN;
//	statePersistenceProbVec[2] = 2*alphaN+2*betaN;
//	statePersistenceProbVec[3] =   alphaN+3*betaN;
//	statePersistenceProbVec[4] =           4*betaN;
//
//	return NT_SUCCESS;
//}
//
//
///* ***  PROTECTED                         ***   */
///* ***  PRIVATE                           ***   */
//
//
///* File skeleton generated by GenNTObj version 0.7. */
