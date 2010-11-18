/**\file ntbp_multi_potassium_current_obj.cpp - squid giant axon sodium conducta nce class implementation
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


/* $Id: ntbp_multi_potassium_current_obj.cpp,v 1.5 2003/01/30 17:13:44 face Exp $
* $Log: ntbp_multi_potassium_current_obj.cpp,v $
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
#include "ntbp_multi_potassium_current_obj.h"

NT_gaussian_rnd_dist_o NTBP_multi_potassium_current_o::normalRnd;
bool NTBP_multi_potassium_current_o::initTableLookUp = false;
NTreal NTBP_multi_potassium_current_o::alphaNvec[15000];
NTreal NTBP_multi_potassium_current_o::betaNvec[15000];

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_multi_potassium_current_o */
NTBP_multi_potassium_current_o::NTBP_multi_potassium_current_o(
    NTreal newArea,
    enum NTBPdelayedPotassiumRectifierType newType,
    NTreal newDensity
)
        :
        NTBP_multi_current_o(-12 /* mV */,0 , newArea, 0) // "0" means to be set in constructor (see below)
{
    NT_ASSERT (newArea >= 0);
    NT_ASSERT (newDensity >= 0);

    type =	newType;
    switch (type)
    {
    case NTBP_SGA_K_CONTI:
        density = 30; // channels per mumeter^2
        conductivity = 0.000000012; // in mSiemens per channel
        break;
    case NTBP_SGA_SCHNEIDMAN:
        density = 18; //channels per mumeter^2
        conductivity = 0.000000020; // in mSiemens per channel
        break;
    case NTBP_SGA_K_LLANO:
        //		break;
    case NTBP_SNAIL:
        //		break;
    case NTBP_FROG_RANVIER1:
        //		break;
    case NTBP_FROG_RANVIER2:
        //		break;
    case NTBP_FROG_RANVIER3:
        //		break;
    case NTBP_FROG_MUSCLE:
        //		break;
    default:
        cerr <<"NTBP_multi_potassium_current_o::NTBP_multi_potassium_current_o - Warning : current type not implemented yet." << endl;
        break;
    };

    if ( 0 != newDensity) density = newDensity;

    NTreal vTmp;
    if (false == initTableLookUp) {
        for (NTsize ll=0; ll < 15000; ll++) {
            vTmp = ((float)ll)/100.0 - 20.0 + 0.005/* otherwise we hit undefined points of alpha and beta function */;
            alphaNvec[ll]= AlphaN(vTmp);
            betaNvec[ll]= BetaN(vTmp);
        }
        initTableLookUp = true;
    }
    ComputeRateConstants(0);
    n = alphaN/(alphaN + betaN);

    noiseN = 0.0;
    UpdateNumChannels();
    channelsPtr = new NTBP_potassium_ion_channels_o( _numChannels() );
    channelsPtr->SteadyStateDistribution();
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_multi_potassium_current_o::NTBP_multi_potassium_current_o(const NTBP_multi_potassium_current_o & original)
        :
        NTBP_multi_current_o(original._reversalPotential(), original._density(), original._area(), original._conductivity())
{
// add assignment code here
    channelsPtr = new NTBP_potassium_ion_channels_o( original._numChannels() );
}

const NTBP_multi_potassium_current_o&
NTBP_multi_potassium_current_o::operator= (const NTBP_multi_potassium_current_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
    channelsPtr = new NTBP_potassium_ion_channels_o( right._numChannels() );
    return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_multi_potassium_current_o::~NTBP_multi_potassium_current_o()
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

inline void
NTBP_multi_potassium_current_o::ComputeRateConstants(NTreal vM)
{
    NTreal q10Factor = NTBP_TemperatureRateRelation(_temperature(), _q10());

    NTsize index = 0;
    if ( (vM < -20.0) || ( vM > 130.0 ) ) {
        alphaN =  q10Factor * AlphaN(vM);
        betaN =  q10Factor * BetaN(vM);
    } else {
        index = (NTsize) floor((vM+20.0)*100.0);
        alphaN =  q10Factor * alphaNvec[index];
        betaN =  q10Factor * betaNvec[index];
    }
}

/* ***  PROTECTED                         ***   */
/*NTreturn DEPRECATED
NTBP_multi_potassium_current_o::SetDensityChild(NTreal channelDensity)// in channels/muMeter^2
{
// 2DO improve : copy the state Besetzungszustaende
	density = channelDensity;
	delete	channelsPtr;
	channelsPtr = new NTBP_potassium_ion_channels_o( density*area );
	return NT_SUCCESS;
}
*/

/** in kHz */
inline NTreal
NTBP_multi_potassium_current_o::ComputeChannelStateTimeConstant() const
{
    return channelsPtr->ComputeChannelStateTimeConstant();
}

/** No descriptions */
inline NTreal
NTBP_multi_potassium_current_o::OpenChannels() const
{
    switch (_simulationMode()) {
    case NTBP_BINOMIALPOPULATION:
    case NTBP_GILLESPIE:
    case NTBP_SINGLECHANNEL:
        return channelsPtr->NumOpen();
        break;
    case NTBP_LANGEVIN:
    case NTBP_DETERMINISTIC:
        return n*n*n*n*NumChannels();
    default:
        cerr << "NTBP_multi_potassium_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels()." << endl;
        return 0;
    }
}


inline NTreal
NTBP_multi_potassium_current_o::ComputeConductance()
{
    switch (_simulationMode()) {
    case NTBP_GILLESPIE:
    case NTBP_SINGLECHANNEL:
    case NTBP_BINOMIALPOPULATION:
        return Set_conductance( channelsPtr->NumOpen() * conductivity);
        break;
    case NTBP_LANGEVIN:
    case NTBP_DETERMINISTIC:
        return Set_conductance(_maxConductivity() /* mS/cm^2 */ * n*n*n*n * _area() /* muMeter^2 */ * 1.0e-8 /* cm^2/muMeter^2 */);
        break;
    case NTBP_NOISYMEAN:
    {
        cerr <<"NOISYMEAN: NOT IMPLEMENTED PROPERLY" << endl;
        NTreal mean = n*n*n*n;
        NTreal numAddOpening = n*n*n*(1-n)* alphaN/_timeStep();
        NTreal numAddClosing = n*n*n*n * 4.0 * betaN/_timeStep();
//					mean = uniformRnd.RndVal()
        return Set_conductance(_maxConductivity() /* mS/cm^2 */ *  mean * _area() /* muMeter^2 */ * 1.0e-8 /* cm^2/muMeter^2 */);
    }
    default:
        cerr << "NTBP_multi_potassium_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance." << endl;
        return 0;
    }
}


inline NTreturn NTBP_multi_potassium_current_o::StepCurrent()
{
    vector < NTreal > vec(2);
    vec[0] = alphaN;
    vec[1] = betaN;

    NTsize counter = 0;
    NTreal tmpN = 0;

    switch (_simulationMode()) {
    case NTBP_BINOMIALPOPULATION:
        NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
        return (channelsPtr->BinomialStep());
        break;
    case NTBP_GILLESPIE:
        return (channelsPtr)->UpdateStateProb(vec);
        break;
    case NTBP_SINGLECHANNEL:
        NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
        return (channelsPtr->Step());
        break;
    case NTBP_LANGEVIN:
        counter = 0;
        n += _timeStep() * ((1.0 - n) * alphaN - n * betaN);
        NT_ASSERT(n>=0 &&  n<= 1);
        do {
            counter++;
            tmpN = _timeStep()*normalRnd.RndVal()
                   * sqrt(
                       (alphaN* (1-n) + betaN * n)
                       /  _numChannels() );
            if (counter > 1 && counter <= 1024)	cerr << "Kn=" <<counter << endl;
            else if (counter > 1024) {
                noiseN = 0;
                tmpN = 0;
                break;
            }
        } while ((_timeStep()*(tmpN + noiseN) + n >= 1) || (_timeStep()*(tmpN + noiseN) + n <= 0));
        noiseN += tmpN;
        n += _timeStep()*noiseN;
        return NT_SUCCESS;
        break;
    case NTBP_NOISYMEAN:
    case NTBP_DETERMINISTIC:
        n += _timeStep() * ((1.0 - n) * alphaN - n * betaN);
        NT_ASSERT(n>=0 &&  n<= 1);
        return NT_SUCCESS;
        break;
    default:
        cerr << "NTBP_multi_potassium_current_o::StepCurrent - ERROR : Unsupported simulation mode." << endl;
        return NT_PARAM_UNSUPPORTED;
    }
}


void NTBP_multi_potassium_current_o::ShowParam() const
{
    cout << "K channel parameters:" << endl;
    cout << "Single channel conductivity [nA]" << _conductivity() << endl;
    cout << "Channel density [1/muMeter^2]" << _area() << endl;
    cout << "MaxConductivity (all channels open) mSiemens/cm^2" << _maxConductivity() << endl;
}

/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
/**  */
/**  */
