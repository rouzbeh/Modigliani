/**\file ntbp_colbert_axonal_sodium_multi_current_obj.cpp - squid giant axon sodium conducta nce class implementation
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


/* $Id: ntbp_colbert_axonal_sodium_multi_current_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
* $Log: ntbp_colbert_axonal_sodium_multi_current_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.2  2003/02/17 10:20:22  face
* *** empty log message ***
*
* Revision 1.1  2003/01/30 17:13:42  face
* *** empty log message ***
*
*/


#include "ntbp_colbert_axonal_sodium_multi_current_obj.h"
NT_gaussian_rnd_dist_o NTBP_colbert_axonal_sodium_multi_current_o::normalRnd;
bool NTBP_colbert_axonal_sodium_multi_current_o::initTableLookUp = false;
NTreal NTBP_colbert_axonal_sodium_multi_current_o::alphaMvec[15000];
NTreal NTBP_colbert_axonal_sodium_multi_current_o::betaMvec[15000];
NTreal NTBP_colbert_axonal_sodium_multi_current_o::alphaHvec[15000];
NTreal NTBP_colbert_axonal_sodium_multi_current_o::betaHvec[15000];

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_colbert_axonal_sodium_multi_current_o */
NTBP_colbert_axonal_sodium_multi_current_o::NTBP_colbert_axonal_sodium_multi_current_o(
				NTreal newArea,
				NTreal newDensity,
				NTreal newConductivity,
				NTreal newVBase,
				NTreal newQ10m, NTreal newQ10h, NTreal reversalPotential
				)
:
	NTBP_multi_current_o(reversalPotential /* in mV */,
						newDensity /* channels per mu^2 */,
						newArea /* in mu^2 */,
						newConductivity /* in mS per channel  */,
						newVBase /* mV */
						)
{
	UpdateNumChannels();
	channelsPtr = new NTBP_sodium_ion_channels_o( _numChannels() );
	noiseM = 0;
	noiseH = 0;
	
	baseTemp = 23.0; // C
	
	/* q10 based on
		Schwartz, J. and Reid, G. and Bostock, H. (1995)
		"Action potentials and membrane currents in the human node of Ranvier"
		J.Physiol. 430:283-292 */
	q10m = newQ10m; //
	q10h = newQ10h; //
	
	NTreal vTmp = 0;
	
	if ( false == initTableLookUp ) {
		for (NTsize ll=0; ll < 15000; ll++){
			vTmp = ((float)ll)/100.0 - 20 + 0.005 /* otherwise we hit undefined points of alpha and beta function */;
			alphaMvec[ll]= AlphaM(vTmp);
			betaMvec[ll]= BetaM(vTmp);
			alphaHvec[ll]= AlphaH(vTmp);
			betaHvec[ll]= BetaH(vTmp);
		}
		initTableLookUp = true;
	}
	
	ComputeRateConstants(0);
	m = alphaM/(alphaM + betaM);
	h = alphaH/(alphaH + betaH);
	
  channelsPtr->SteadyStateDistribution();
		
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_colbert_axonal_sodium_multi_current_o::NTBP_colbert_axonal_sodium_multi_current_o(const NTBP_colbert_axonal_sodium_multi_current_o & original)
:
NTBP_multi_current_o(original._reversalPotential(), original._density(), original._area(), original._conductivity())
{
	channelsPtr = new NTBP_sodium_ion_channels_o(  original._numChannels() );
}

const NTBP_colbert_axonal_sodium_multi_current_o&
NTBP_colbert_axonal_sodium_multi_current_o::operator= (const NTBP_colbert_axonal_sodium_multi_current_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
	channelsPtr = new NTBP_sodium_ion_channels_o( right._numChannels()  );
  return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_colbert_axonal_sodium_multi_current_o::~NTBP_colbert_axonal_sodium_multi_current_o()
{
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
inline NTreturn NTBP_colbert_axonal_sodium_multi_current_o::StepCurrent()
{
//	cerr << "NTBP_colbert_axonal_sodium_multi_current_o::StepCurrent()" << endl;
	NTreal tmpM = 0;
	NTreal tmpH = 0;	
	NTsize counter = 0;

	switch(_simulationMode()) {
		case NTBP_BINOMIALPOPULATION:
					{
					vector < NTreal > vec(4);
					vec[0] = alphaM;
					vec[1] = betaM;
   		    vec[2] = alphaH;
					vec[3] = betaH;
					NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
					return (channelsPtr->BinomialStep());			
					}
					break;
		case NTBP_GILLESPIE:
					{
					vector < NTreal > vec(4);
					vec[0] = alphaM;
					vec[1] = betaM;
   				vec[2] = alphaH;
					vec[3] = betaH;
					return (channelsPtr)->UpdateStateProb(vec);
					}
					break;
		case NTBP_SINGLECHANNEL:
					{
					vector < NTreal > vec(4);
					vec[0] = alphaM;
   				vec[1] = betaM;
		     	vec[2] = alphaH;
					vec[3] = betaH;
					NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
					return (channelsPtr->Step());			
					}
				break;
		case NTBP_LANGEVIN:
					{
					counter = 0;
					m += _timeStep() * ((1.0 - m) * alphaM - m * betaM);
					NT_ASSERT(m>=0 &&  m<=1);								
					do {
						counter++;
						tmpM = _timeStep()*normalRnd.RndVal()* sqrt((alphaM * (1-m) + betaM * m)/_numChannels());
						if (counter > 1 && counter < 1024)	cerr << "NaM=" << counter << endl;
						else if (counter >= 1024 ) {noiseM = 0;tmpM=0; break;}
					} while (( _timeStep()*(tmpM + noiseM) + m >= 1) || (_timeStep()*(tmpM+noiseM) + m <= 0));
					noiseM += tmpM;
					m += _timeStep()*noiseM;
						
					counter = 0;
					h += _timeStep() * ((1.0 - h) * alphaH - h * betaH);
					NT_ASSERT(h>=0 &&  h<= 1);			
					do {
						counter++;
						tmpH = _timeStep()*normalRnd.RndVal()*sqrt((alphaH* (1-h) + betaH * h)/_numChannels());
						if (counter > 1 && counter < 1024)	 cerr << "NaH=" << counter << endl;
						else if (counter >= 1024){ noiseH = 0;tmpH=0;break;}
					} while ((_timeStep()*(tmpH + noiseH) + h >= 1) || (_timeStep()*(tmpH + noiseH) + h <= 0));					
					noiseH += tmpH;
					h += _timeStep()*noiseH;
					
					return NT_SUCCESS;
					}
				break;
		case NTBP_NOISYMEAN:
		case NTBP_DETERMINISTIC:
					m += _timeStep() * ((1.0 - m) * alphaM - m * betaM);
					NT_ASSERT(m>=0 &&  m<= 1);			
					h += _timeStep() * ((1.0 - h) * alphaH - h * betaH);
					NT_ASSERT(h>=0 &&  h<= 1);			
					return NT_SUCCESS;
				break;
		default:
				cerr << "NTBP_colbert_axonal_sodium_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode." << endl;
				return NT_PARAM_UNSUPPORTED;
				break;
	}
	return NT_FAIL;
}



inline void
NTBP_colbert_axonal_sodium_multi_current_o::ComputeRateConstants(NTreal vM /* in mV */)
{
	//	cerr << "NTBP_colbert_axonal_sodium_multi_current_o::ComputeRateConstants" << endl;
	NTreal temp = _temperature();
	
	NTreal q10Factor = 1.0;
//NTsize index = 0;
//if ( (vM < -20) || ( vM > 130.0 ) ) {
		q10Factor = NTBP_TemperatureRateRelation(temp, baseTemp /* C */, q10m);
		alphaM = q10Factor * AlphaM(vM);
		betaM = q10Factor * BetaM(vM);
		
		q10Factor = NTBP_TemperatureRateRelation(temp, baseTemp /* C */, q10h);
		alphaH = q10Factor * AlphaH(vM);
		betaH = q10Factor * BetaH(vM);
/*} else {
		index = (NTsize) floor((vM+20)*100.0);
		q10Factor = NTBP_TemperatureRateRelation(temp, baseTemp /* C *, q10m);
		alphaM = q10Factor * alphaMvec[index];
		betaM = q10Factor * betaMvec[index];
		
		q10Factor = NTBP_TemperatureRateRelation(temp, baseTemp /* C *, q10h);
		alphaH = q10Factor * alphaHvec[index];
		betaH = q10Factor * betaHvec[index];
	}
	*/
}

/**  */
/** No descriptions */
inline NTreal
NTBP_colbert_axonal_sodium_multi_current_o::OpenChannels() const
{
	switch(_simulationMode()) {
		case NTBP_BINOMIALPOPULATION:
		case NTBP_GILLESPIE:
		case NTBP_SINGLECHANNEL:
				return channelsPtr->NumOpen();
				break;
		case NTBP_LANGEVIN:
		case NTBP_DETERMINISTIC:
				return m*m*m*h*NumChannels();
		default:
				cerr << "NTBP_colbert_axonal_sodium_multi_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels." << endl;
				return 0;			}	
}


inline NTreal NTBP_colbert_axonal_sodium_multi_current_o::ComputeConductance()
{
//  cerr << "NTBP_colbert_axonal_sodium_multi_current_o::ComputeConductance" << endl;
	switch(_simulationMode()) {
		case NTBP_BINOMIALPOPULATION:
		case NTBP_GILLESPIE:
		case NTBP_SINGLECHANNEL:
				return Set_conductance( channelsPtr->NumOpen() * conductivity);
				break;
		case NTBP_LANGEVIN:
		case NTBP_DETERMINISTIC:
				return Set_conductance(_maxConductivity() /* mS/cm^2 */ * m*m*m*h * _area() /* muMeter^2 */ * 1.0e-8 /* cm^2/muMeter^2 */);	
				break;
		case NTBP_NOISYMEAN:
				{
					NTreal mean = m*m*m*h;
					NTreal numAddOpening = m*m*(1-m)*(1-h)* alphaM/_timeStep() + m*m*m*(1-h) * alphaH/_timeStep();
					NTreal numAddClosing = m*m*m*h * (3 * betaM + betaH)/_timeStep();
					mean += numAddOpening - numAddClosing;
					cerr << "NOT IMPLEMENTED CORRECTLY"<< endl;
					return Set_conductance(_maxConductivity() /* mS/cm^2 */ *  mean * _area() /* muMeter^2 */ * 1.0e-8 /* cm^2/muMeter^2 */);					
				break;
				}
		default:
				cerr << "NTBP_colbert_axonal_sodium_multi_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance." << endl;
				return 0;
	}
}


inline NTreal
NTBP_colbert_axonal_sodium_multi_current_o::ComputeChannelStateTimeConstant() const
{
  // cerr << "NTBP_colbert_axonal_sodium_multi_current_o::ComputeChannelStateTimeConstant()" << endl;
	return channelsPtr->ComputeChannelStateTimeConstant();
}

void NTBP_colbert_axonal_sodium_multi_current_o::ShowParam() const
{
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2" << _maxConductivity() << endl;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */


