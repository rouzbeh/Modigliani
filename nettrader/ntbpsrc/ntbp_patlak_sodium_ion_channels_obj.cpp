/**\file ntbp_patlak_sodium_ion_channels_obj.cpp - NTBP_patlak_sodium_ion_channels_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 12.1.2003  
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
  

/* $Id: ntbp_patlak_sodium_ion_channels_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_patlak_sodium_ion_channels_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2003/01/30 17:13:44  face
* *** empty log message ***
*

*/
#include "ntbp_patlak_sodium_ion_channels_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_patlak_sodium_ion_channels_o */
NTBP_patlak_sodium_ion_channels_o::NTBP_patlak_sodium_ion_channels_o(NTsize numNewChannels)
:
NTBP_ion_channels_o(numNewChannels, 7)
{
	for(NTsize ll=0; ll < _numStates()+1; ll++) stateCounterVec[ll] = 0;
	stateCounterVec[0] = _numChannels();
	stateCounterVec[1] = _numChannels();
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_patlak_sodium_ion_channels_o::NTBP_patlak_sodium_ion_channels_o(const NTBP_patlak_sodium_ion_channels_o & original)
{
 // add assignment code here
}

const NTBP_patlak_sodium_ion_channels_o&  
NTBP_patlak_sodium_ion_channels_o::operator= (const NTBP_patlak_sodium_ion_channels_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_patlak_sodium_ion_channels_o::~NTBP_patlak_sodium_ion_channels_o()
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
NTBP_patlak_sodium_ion_channels_o::Step()
{
	NTreal rv = 0;
							
	vector<NTsize> oldStateCounterVec = stateCounterVec;
	
	for(NTsize lls=1; lls < _numStates()+1; lls++){
		for(NTsize llc=0; llc < oldStateCounterVec[lls]; llc++){
		rv = uniformRnd.RndVal();
		switch (lls)
			{
			case 1:	 if (rv <= p12) {stateCounterVec[2]++;stateCounterVec[1]--;}
				break;
			case 2:	 if (rv <= p23) {stateCounterVec[3]++;stateCounterVec[2]--;}
			 else if (rv <= t21) {stateCounterVec[1]++;stateCounterVec[2]--;}
				break;
			case 3:  if (rv <= p34) {stateCounterVec[4]++;stateCounterVec[3]--;}
				else if (rv <= t36) {stateCounterVec[6]++;stateCounterVec[3]--;}
				else if (rv <= t32) {stateCounterVec[2]++;stateCounterVec[3]--;}
				break;
			case 4:  if (rv <= p45) {stateCounterVec[5]++;stateCounterVec[4]--;}
				else if (rv <= t46) {stateCounterVec[6]++;stateCounterVec[4]--;}
				else if (rv <= t43) {stateCounterVec[3]++;stateCounterVec[4]--;}
				break;
			case 5:  if (rv <= p57) {stateCounterVec[7]++;stateCounterVec[5]--;}
				else if (rv <= t54) {stateCounterVec[4]++;stateCounterVec[5]--;}
				break;
			case 6:  if (rv <= p63) {stateCounterVec[3]++;stateCounterVec[6]--;}
				else if (rv <= t64) {stateCounterVec[4]++;stateCounterVec[6]--;}
				else if (rv <= t67) {stateCounterVec[7]++;stateCounterVec[6]--;}
				break;
			case 7:  if (rv <= p76) {stateCounterVec[6]++;stateCounterVec[7]--;}
				else if (rv <= t75) {stateCounterVec[5]++;stateCounterVec[7]--;}
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
NTBP_patlak_sodium_ion_channels_o::ShowStates() const
{
	cout << "\tNaChannel:";
	for(NTsize ll=1; ll < _numStates()+1; ll++) cout << stateCounterVec[ll] << " ";
}



/** Sum of escape rates [1/s] */
NTreal
NTBP_patlak_sodium_ion_channels_o::ComputeChannelStateTimeConstant() const
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


/** @short
    @param      rateConstVec  = [ alphaM, betaM, alphaH, alphaH ] from std. HH naming convention
    @return     none
   \warning    unknown
   \bug        unknown
 */
inline NTreturn
NTBP_patlak_sodium_ion_channels_o::UpdateStateProb( vector < NTreal > rateConstVec )
{
//	cerr << "NTBP_sodium_ion_channels_o::UpdateStateProb called" << endl;
		
	NT_ASSERT( rateConstVec.size() == 10);
	NTreal deltaT = _timeStep();

	vector < NTreal > transProbVec = rateConstVec;
	for (NTsize ll=0;ll <transProbVec.size();ll++){
		transProbVec[ll] = deltaT*rateConstVec[ll];	
  }

		
	alphaMP = transProbVec[0];
  betaMP = transProbVec[1];
	
	alphaHP = transProbVec[2];
	betaHP = transProbVec[3];

	gammaP = transProbVec[4];
	deltaP = transProbVec[5];
	
	kicP = transProbVec[6];
	kciP = transProbVec[7];
	
	kappaP = transProbVec[8];
	lambdaP = transProbVec[9];
	
	 p12 = 4*kappaP;
	 p21 = lambdaP;
	
	 p23 = 3*alphaMP;
	 p32 = 2*betaMP;
	
	 p34 = 2*alphaMP;
	 p43 = 3*betaMP;
	
	 p45 = deltaP;
	 p54 = 4*gammaP;
	 	
	 p67 = p45;
	 p76 = p54;
	
	 p36 = kciP;
	 p63 = kicP;
	
	 p46 = betaHP;
	 p64 = alphaHP;
	
	 p57 = p46;
	 p75 = p64;


	 	
	 t21 = p23+p21;
	
	 t36 = p45+p36;
	 t32 = t36+p32;
		
	 t46 = p45+p46;
	 t43 = t46+p43;
		
	 t54 = p57+p54;
	
	 t64 = p63+p64;
	 t67 = t64+p67;
	
   t75 = p76+p75;
	
  // this vector contains the sum of the escape RATES
	statePersistenceProbVec[0] = p12/deltaT;
	statePersistenceProbVec[1] = t21/deltaT;
	statePersistenceProbVec[2] = t32/deltaT;
	statePersistenceProbVec[3] = t43/deltaT;
	statePersistenceProbVec[4] = t54/deltaT;
	statePersistenceProbVec[5] = t67/deltaT;	
	statePersistenceProbVec[6] = t75/deltaT;
		
	return NT_SUCCESS;
}


inline NTreturn
NTBP_patlak_sodium_ion_channels_o::SteadyStateDistribution()
{
	for (NTsize ll=0; ll<_numChannels(); ll++){
		Step();	
	}
	return NT_SUCCESS;
}


/** No descriptions */
inline NTreturn
NTBP_patlak_sodium_ion_channels_o::BinomialStep()
{
	vector<NTsize> oldStateCounterVec = stateCounterVec;
  vector<NTint> newStateCounterVec(stateCounterVec.size());
  for(NTsize ll=1; ll < _numStates()+1; ll++)	{
		newStateCounterVec[ll] = stateCounterVec[ll];
	}
		
  bool loop = false;
	NTsize loopCounter = 0;

  do {
	loop = false;
	loopCounter++;
	
	NTreal delta12 = binomRnd.Binomial(p12, oldStateCounterVec[1]);
	newStateCounterVec[2] += delta12;
	newStateCounterVec[1] -= delta12;
	
	NTreal delta23 = binomRnd.Binomial(p23, oldStateCounterVec[2]);
	newStateCounterVec[3] += delta23;
	newStateCounterVec[2] -= delta23;
	NTreal delta21 = binomRnd.Binomial(p21, oldStateCounterVec[2]);
	newStateCounterVec[1] += delta21;
	newStateCounterVec[2] -= delta21;
	
	NTreal delta34 = binomRnd.Binomial(p34, oldStateCounterVec[3]);
	newStateCounterVec[4] += delta34;
	newStateCounterVec[3] -= delta34;
	NTreal delta36 = binomRnd.Binomial(p36, oldStateCounterVec[3]);
	newStateCounterVec[6] += delta36;
	newStateCounterVec[3] -= delta36;
	NTreal delta32 = binomRnd.Binomial(p32, oldStateCounterVec[3]);
	newStateCounterVec[2] += delta32;
	newStateCounterVec[3] -= delta32;
		
	NTreal delta45 = binomRnd.Binomial(p45, oldStateCounterVec[4]);
	newStateCounterVec[5] += delta45;
	newStateCounterVec[4] -= delta45;
	NTreal delta46 = binomRnd.Binomial(p46, oldStateCounterVec[4]);
	newStateCounterVec[6] += delta46;
	newStateCounterVec[4] -= delta46;
	NTreal delta43 = binomRnd.Binomial(p43, oldStateCounterVec[4]);
	newStateCounterVec[3] += delta43;
	newStateCounterVec[4] -= delta43;
		
	NTreal delta54 = binomRnd.Binomial(p54, oldStateCounterVec[5]);
	newStateCounterVec[4] += delta54;
	newStateCounterVec[5] -= delta54;
	NTreal delta57 = binomRnd.Binomial(p57, oldStateCounterVec[5]);
	newStateCounterVec[7] += delta57;
	newStateCounterVec[5] -= delta57;
		
	NTreal delta63 = binomRnd.Binomial(p63, oldStateCounterVec[6]);
	newStateCounterVec[3] += delta63;
	newStateCounterVec[6] -= delta63;
  NTreal delta64 = binomRnd.Binomial(p64, oldStateCounterVec[6]);
	newStateCounterVec[4] += delta64;
	newStateCounterVec[6] -= delta64;
	NTreal delta67 = binomRnd.Binomial(p67, oldStateCounterVec[6]);
	newStateCounterVec[7] += delta67;
	newStateCounterVec[6] -= delta67;
	
	NTreal delta76 = binomRnd.Binomial(p76, oldStateCounterVec[7]);
	newStateCounterVec[6] += delta76;
	newStateCounterVec[7] -= delta76;
	NTreal delta75 = binomRnd.Binomial(p75, oldStateCounterVec[7]);
	newStateCounterVec[5] += delta75;
	newStateCounterVec[7] -= delta75;
	
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
