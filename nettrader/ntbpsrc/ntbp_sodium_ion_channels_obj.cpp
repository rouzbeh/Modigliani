/**\file ntbp_sodium_ion_channels_obj.cpp - NTBP_sodium_ion_channels_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 15.3.2001  
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
  

/* $Id: ntbp_sodium_ion_channels_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_sodium_ion_channels_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.7  2003/01/30 17:13:45  face
* *** empty log message ***
*
* Revision 1.6  2002/12/12 15:31:45  face
* *** empty log message ***
*
* Revision 1.5  2002/03/13 19:23:27  face
* *** empty log message ***
*
* Revision 1.4  2002/03/08 15:13:26  face
* *** empty log message ***
*
* Revision 1.3  2002/03/04 18:09:07  face
* added checking for NaN and Inf values in find_spikes_in_binary_file-
* BinomialStep: redo loop if after state change the states do not sum up
*
* Revision 1.2  2001/10/12 09:18:36  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#include "ntbp_sodium_ion_channels_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_sodium_ion_channels_o */
NTBP_sodium_ion_channels_o::NTBP_sodium_ion_channels_o(NTsize numNewChannels)
:
NTBP_ion_channels_o(numNewChannels, 8)
{
	for(NTsize ll=0; ll < _numStates()+1; ll++) stateCounterVec[ll] = 0;
	stateCounterVec[0] = _numChannels();
	stateCounterVec[1] = _numChannels();
}

/* ***      COPY AND ASSIGNMENT	***/ 
NTBP_sodium_ion_channels_o::NTBP_sodium_ion_channels_o(const NTBP_sodium_ion_channels_o & original)
{
 // add assignment code here
}

const NTBP_sodium_ion_channels_o&  
NTBP_sodium_ion_channels_o::operator= (const NTBP_sodium_ion_channels_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_sodium_ion_channels_o::~NTBP_sodium_ion_channels_o()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
NTreturn
NTBP_sodium_ion_channels_o::Step()
{
	NTreal rv = 0;
							
	vector<NTsize> oldStateCounterVec = stateCounterVec;
	
	for(NTsize lls=1; lls < _numStates()+1; lls++){
		for(NTsize llc=0; llc < oldStateCounterVec[lls]; llc++){
		rv = uniformRnd.RndVal();
		switch (lls)
			{
			case 1:	 if (rv <= p12) {stateCounterVec[2]++;stateCounterVec[1]--;}
				else if (rv <= t15) {stateCounterVec[5]++;stateCounterVec[1]--;}
				break;
			case 2:	 if (rv <= p23) {stateCounterVec[3]++;stateCounterVec[2]--;}
				else if (rv <= t26) {stateCounterVec[6]++;stateCounterVec[2]--;}
				else if (rv <= t21) {stateCounterVec[1]++;stateCounterVec[2]--;}
				break;
			case 3:  if (rv <= p34) {stateCounterVec[4]++;stateCounterVec[3]--;}
				else if (rv <= t37) {stateCounterVec[7]++;stateCounterVec[3]--;}
				else if (rv <= t32) {stateCounterVec[2]++;stateCounterVec[3]--;}
				break;
			case 4:  if (rv <= p43) {stateCounterVec[3]++;stateCounterVec[4]--;}
				else if (rv <= t48) {stateCounterVec[8]++;stateCounterVec[4]--;}
				break;
			case 5:  if (rv <= p56) {stateCounterVec[6]++;stateCounterVec[5]--;}
				else if (rv <= t51) {stateCounterVec[1]++;stateCounterVec[5]--;}
				break;
			case 6:  if (rv <= p67) {stateCounterVec[7]++;stateCounterVec[6]--;}
				else if (rv <= t62) {stateCounterVec[2]++;stateCounterVec[6]--;}
				else if (rv <= t65) {stateCounterVec[5]++;stateCounterVec[6]--;}
				break;
			case 7:  if (rv <= p78) {stateCounterVec[8]++;stateCounterVec[7]--;}
				else if (rv <= t73) {stateCounterVec[3]++;stateCounterVec[7]--;}
				else if (rv <= t76) {stateCounterVec[6]++;stateCounterVec[7]--;}
				break;
			case 8:  if (rv <= p87) {stateCounterVec[7]++;stateCounterVec[8]--;}
				else if (rv <= t84) {stateCounterVec[4]++;stateCounterVec[8]--;}
				break;
			default:
				cerr <<"NTBP_sodium_ion_channels_o::Step() - Warning : Invalid state in channel index "<<llc<<" , state "<<lls<<" encountered. State unchanged" << endl;
				return NT_FAIL;
				break;
			}
	
		}
	}
	return NT_SUCCESS;
}

void
NTBP_sodium_ion_channels_o::ShowStates() const
{
	cout << "\tNaChannel:";
	for(NTsize ll=1; ll < _numStates()+1; ll++) cout << stateCounterVec[ll] << " ";
}



/** Sum of escape rates [1/s] */
NTreal
NTBP_sodium_ion_channels_o::ComputeChannelStateTimeConstant() const
{
	cerr << "NTBP_sodium_ion_channels_o::ComputeChannelStateTimeConstant()" << endl;
	NTreal sum = 0.0;
	for (NTsize ll = 0; ll < _numStates(); ll++) 	{
//		cerr << "NA_ION_CHANNELS: statePersistenceProbVec["<<ll<<"]="<<statePersistenceProbVec[ll] << ",stateCounterVec["<<ll+1<<"]="<< stateCounterVec[ll + 1] << endl;
		sum +=	statePersistenceProbVec[ll] * stateCounterVec[ll + 1];
	}
	//  cerr << " Na " << endl;
	return 	sum;
}

/**  */
bool NTBP_sodium_ion_channels_o::ComputeGillespieStep(NTsize stateId)
{
  cerr << "NTBP_potassium_ion_channels_o::ComputeGillespieStep" << endl;
  NT_uniform_rnd_dist_o rnd;

	NTsize	oldOpen = NumOpen();

	NTreal val = rnd.RndVal()*_timeStep();
	// the probability has to be converted into a rate
	//	cout << "NaChannel (of which"<< NumOpen() <<"open) state to switch "<< stateId << "with 'rate'" << val << "nd state_1_1"	<< p12/statePersistenceProbVec[0] << endl;
	//	ShowStates();
	switch (stateId)
			{
			case 1:	 if (val < p12/statePersistenceProbVec[0]) {stateCounterVec[2]++;stateCounterVec[1]--;}
				else  {stateCounterVec[5]++;stateCounterVec[1]--;}
				break;
			case 2:	 if (val < p23/statePersistenceProbVec[1]) {stateCounterVec[3]++;stateCounterVec[2]--;}
				else if (val < t26/statePersistenceProbVec[1]) {stateCounterVec[6]++;stateCounterVec[2]--;}
				else {stateCounterVec[1]++;stateCounterVec[2]--;}
				break;
			case 3:  if (val < p34/statePersistenceProbVec[2]) {stateCounterVec[4]++;stateCounterVec[3]--;}
				else if (val < t37/statePersistenceProbVec[2]) {stateCounterVec[7]++;stateCounterVec[3]--;}
				else {stateCounterVec[2]++;stateCounterVec[3]--;}
				break;
			case 4:  if (val < p43/statePersistenceProbVec[3]) {stateCounterVec[3]++;stateCounterVec[4]--;}
				else  {stateCounterVec[8]++;stateCounterVec[4]--;}
				break;
			case 5:  if (val < p56/statePersistenceProbVec[4]) {stateCounterVec[6]++;stateCounterVec[5]--;}
				else {stateCounterVec[1]++;stateCounterVec[5]--;}
				break;
			case 6:  if (val < p67/statePersistenceProbVec[5]) {stateCounterVec[7]++;stateCounterVec[6]--;}
				else if (val < t62/statePersistenceProbVec[5]) {stateCounterVec[2]++;stateCounterVec[6]--;}
				else {stateCounterVec[5]++;stateCounterVec[6]--;}
				break;
			case 7:  if (val < p78/statePersistenceProbVec[6]) {stateCounterVec[8]++;stateCounterVec[7]--;}
				else if (val < t73/statePersistenceProbVec[6]) {stateCounterVec[3]++;stateCounterVec[7]--;}
				else  {stateCounterVec[6]++;stateCounterVec[7]--;}
				break;
			case 8:  if (val < p87/statePersistenceProbVec[7]) {stateCounterVec[7]++;stateCounterVec[8]--;}
				else {stateCounterVec[4]++;stateCounterVec[8]--;}
				break;
			default:
				cerr << "NTBP_sodium_ion_channels_o::ComputeGillespieStep - Error : Invalid stateId specified." << endl;
				return NT_FAIL;
	}
	//	ShowStates();
	/* CHECKING CODE */
	NTint check = 0.0;
	for(NTsize ll=1; ll < _numStates()+1; ll++) {
		check += stateCounterVec[ll];
		cerr << check << endl;
	}
	cerr << _numChannels() << endl;
	NT_ASSERT(check == _numChannels());

	if (NumOpen() == oldOpen) return false;
	else return true;
}


/** @short
    @param      rateConstVec  = [ alphaM, betaM, alphaH, alphaH ] from std. HH naming convention
    @return     none
   \warning    unknown
   \bug        unknown
 */
inline NTreturn
NTBP_sodium_ion_channels_o::UpdateStateProb( vector < NTreal > rateConstVec )
{
//	cerr << "NTBP_sodium_ion_channels_o::UpdateStateProb called" << endl;
		
	NT_ASSERT( rateConstVec.size() == 4);
	NTreal deltaT = _timeStep();

	
	alphaM = rateConstVec[0];
  betaM = rateConstVec[1];
	alphaH = rateConstVec[2];
	betaH = rateConstVec[3];

	NTreal alphaMdeltaT = alphaM*deltaT;
	NTreal betaMdeltaT =  betaM*deltaT;
	NTreal alphaHdeltaT = alphaH*deltaT;
	NTreal betaHdeltaT = betaH*deltaT;
	
	 p12 = 3*alphaMdeltaT;
	 p23 = 2*alphaMdeltaT;
	 p34 =   alphaMdeltaT;
	 p56 = p12;
	 p67 = p23;
	 p78 = p34;

	 p87 = 3*betaMdeltaT;
	 p76 = 2*betaMdeltaT;
	 p65 =   betaMdeltaT;
	 p43 = p87;
	 p32 = p76;
	 p21 = p65;

   p15 = betaHdeltaT;
 	 p26 = betaHdeltaT;
 	 p37 = betaHdeltaT;
	 p48 = betaHdeltaT;
	
	 p51 = alphaHdeltaT;
	 p62 = alphaHdeltaT;
	 p73 = alphaHdeltaT;
	 p84 = alphaHdeltaT;
	 	
	 t15 = p12 + p15;
	 t26 = p23 + p26;
	 t37 = p34 + p37;
	 t48 = p43 + p48;

	 t51 = p56 + p51;
	 t62 = p67 + p62;
	 t73 = p78 + p73;
	 t84 = p87 + p84;
	
	 t21 = t26 + p21;
	 t32 = t37 + p32;
	 t65 = t62 + p65;
	 t76 = t73 + p76;
	
	
	 // this vector contains the sum of the escape RATES
	statePersistenceProbVec[0] = t15/deltaT;
	statePersistenceProbVec[1] = t21/deltaT;
	statePersistenceProbVec[2] = t32/deltaT;
	statePersistenceProbVec[3] = t48/deltaT;
	statePersistenceProbVec[4] = t51/deltaT;
	statePersistenceProbVec[5] = t65/deltaT;	
	statePersistenceProbVec[6] = t76/deltaT;
	statePersistenceProbVec[7] = t84/deltaT;
		
	return NT_SUCCESS;
}

/** Do ot call before rate constants were set. */#
inline NTreturn
NTBP_sodium_ion_channels_o::SteadyStateDistribution()
{
//2DO dirty hack ... make time step dependent and maybe compute directly state
//values
	for (NTsize ll=0; ll< 1000; ll++){
		BinomialStep();		//much faster then having to wait for Step()
	}
	return NT_SUCCESS;
}


/** No descriptions */
inline NTreturn
NTBP_sodium_ion_channels_o::BinomialStep()
{
	vector<NTsize> oldStateCounterVec = stateCounterVec;
  vector<NTint> newStateCounterVec(stateCounterVec.size());
  for(NTsize ll=1; ll < _numStates()+1; ll++)	{
		newStateCounterVec[ll] = stateCounterVec[ll];
	}
		
  bool loop = false;
	NTsize loopCounter = 0;

  do{
	loop = false;
	loopCounter++;
	NTreal delta12 = binomRnd.Binomial(p12, oldStateCounterVec[1]);
	newStateCounterVec[2] += delta12;
	newStateCounterVec[1] -= delta12;
	NTreal delta23 = binomRnd.Binomial(p23,oldStateCounterVec[2]);
	newStateCounterVec[3] += delta23;
	newStateCounterVec[2] -= delta23;
	NTreal delta34 = binomRnd.Binomial(p34,  oldStateCounterVec[3]);
	newStateCounterVec[4] += delta34;
	newStateCounterVec[3] -= delta34;
	// ->
	NTreal delta56 =  binomRnd.Binomial(p56,  oldStateCounterVec[5]);
	newStateCounterVec[6] += delta56;
	newStateCounterVec[5] -= delta56;
	NTreal delta67 = binomRnd.Binomial(p67, oldStateCounterVec[6]);
	newStateCounterVec[7] += delta67;
	newStateCounterVec[6] -= delta67;
	NTreal delta78 = binomRnd.Binomial(p78,  oldStateCounterVec[7]);
	newStateCounterVec[8] += delta78;
	newStateCounterVec[7] -= delta78;
	// <-
	NTreal delta21 =  binomRnd.Binomial(p21,  oldStateCounterVec[2]);
	newStateCounterVec[1] += delta21;
	newStateCounterVec[2] -= delta21;
	NTreal delta32 =  binomRnd.Binomial(p32,  oldStateCounterVec[3]);
	newStateCounterVec[2] += delta32;
	newStateCounterVec[3] -= delta32;
	NTreal delta43 =  binomRnd.Binomial(p43,  oldStateCounterVec[4]);
	newStateCounterVec[3] += delta43;
	newStateCounterVec[4] -= delta43;
	// <-
	NTreal delta65 =  binomRnd.Binomial(p65,  oldStateCounterVec[6]);
	newStateCounterVec[5] += delta65;
	newStateCounterVec[6] -= delta65;
	NTreal delta76 =  binomRnd.Binomial(p76,  oldStateCounterVec[7]);
	newStateCounterVec[6] += delta76;
	newStateCounterVec[7] -= delta76;
	NTreal delta87 =  binomRnd.Binomial(p87,  oldStateCounterVec[8]);
	newStateCounterVec[7] += delta87;
	newStateCounterVec[8] -= delta87;
	// V	
	NTreal delta15 = binomRnd.Binomial(p15,  oldStateCounterVec[1]);
	newStateCounterVec[5] += delta15;
	newStateCounterVec[1] -= delta15;
	NTreal delta26 = binomRnd.Binomial(p26,  oldStateCounterVec[2]);
	newStateCounterVec[6] += delta26;
	newStateCounterVec[2] -= delta26;
	NTreal delta37 = binomRnd.Binomial(p37,  oldStateCounterVec[3]);
	newStateCounterVec[7] += delta37;
	newStateCounterVec[3] -= delta37;
	NTreal delta48 = binomRnd.Binomial(p48,  oldStateCounterVec[4]);
	newStateCounterVec[8] += delta48;
	newStateCounterVec[4] -= delta48;
	// ^
	NTreal delta51 = binomRnd.Binomial(p51,  oldStateCounterVec[5]);
	newStateCounterVec[1] += delta51;
	newStateCounterVec[5] -= delta51;
	NTreal delta62 = binomRnd.Binomial(p62, oldStateCounterVec[6]);
	newStateCounterVec[2] += delta62;
	newStateCounterVec[6] -= delta62;
	NTreal delta73 = binomRnd.Binomial(p73,  oldStateCounterVec[7]);
	newStateCounterVec[3] += delta73;
	newStateCounterVec[7] -= delta73;
	NTreal delta84 = binomRnd.Binomial(p84,  oldStateCounterVec[8]);
	newStateCounterVec[4] += delta84;
	newStateCounterVec[8] -= delta84;
		
	/* CHECKING CODE */
  NTsize check=0;
	for(NTsize ll=1; ll < _numStates()+1; ll++)	{
		check += newStateCounterVec[ll];
		/** if this bails out here it means that the step sizes (i.e. probabilities) are to large */
	  if	(newStateCounterVec[ll] < 0 ) loop = true;
		}
  if (check != _numChannels()) loop = true;
	

  // after 100 attempts to find an adequate distribution,
  // ignore any transitions and "skip" this step
	} while ((true == loop) && (loopCounter < 100));
	if (loopCounter < 100) {
		for(NTsize ll=1; ll < _numStates()+1; ll++)	{
			stateCounterVec[ll] = newStateCounterVec[ll];
		}
	}
//	NT_ASSERT(newStateCounterVec.size() == 9);
//	NT_ASSERT(check == _numChannels());
	return NT_SUCCESS;
}


/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
