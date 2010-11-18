/**\file ntbp_patlak_sodium_multi_current_obj.cpp - NTBP_patlak_sodium_multi_current_o class implementation
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


/* $Id: ntbp_patlak_sodium_multi_current_obj.cpp,v 1.3 2003/06/02 13:22:13 face Exp $
* $Log: ntbp_patlak_sodium_multi_current_obj.cpp,v $
* Revision 1.3  2003/06/02 13:22:13  face
* *** empty log message ***
*
* Revision 1.2  2003/01/30 17:13:44  face
* *** empty log message ***
*
* Revision 1.1  2003/01/17 16:56:14  face
* *** empty log message ***
*
*/

#include "ntbp_patlak_sodium_multi_current_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_patlak_sodium_multi_current_o */
NTBP_patlak_sodium_multi_current_o::NTBP_patlak_sodium_multi_current_o(NTreal newArea, NTreal newDensity, NTreal newConductivity)
        :
        NTBP_multi_current_o(115 /* in mV */, //2DO IS THIS THE RIGHT APPROACH VALUES ARE NOT FOR THESE VOLTAGES
                             newDensity /* channels per mu^2 */,
                             newArea /* in mu^2 */,
                             newConductivity /* in mS per channel  */,
                             -70 /* mV */
                            )
{
// general
    maxRate= 8;//[ms^-1] // Patlak(1991) 8.0 // Maconochie (1995) 333
// \alpha_m
    w_am= -19.0;
    z_am= 2.45;
    d_am= 0.6;
// \beta_m
    w_bm= -22.35;
    z_bm= 2.45;
    d_bm= 0.6;
// \alpha_h
    w_ah= -26.5;
    z_ah= 0.3;
    d_ah= 0.5;
// \beta_h
    w_bh= -22.4;
    z_bh= 0.3;
    d_bh= 0.5;
// k_{ci}
    w_kci= -21.0;
    z_kci= 2.45;
    d_kci= 0.55;
// k_{ic}
    w_kic= -28.05;
    z_kic= 2.45;
    d_kic= 0.55;
// \delta
    w_d= -19.0;
    del_d= -1.8;
    z_d= 2.45;
    d_d= 0.6;
// \gamma
    w_g= -23.9;
    del_g= -1.8;
    z_g= 2.45;
    d_g= 0.6;
// \kappa
    w_k= -21.32;
    del_k= -1.8;
    z_k= 2.45;
    d_k= 0.6;
// \lambda
    w_l= -27.08;
    del_l= -1.8;
    z_l= 2.45;
    d_l= 0.6;


    UpdateNumChannels();
    channelsPtr = new NTBP_patlak_sodium_ion_channels_o( _numChannels() );

    ComputeRateConstants(-65.0);
    channelsPtr->SteadyStateDistribution();

}


/* ***      COPY AND ASSIGNMENT	***/
NTBP_patlak_sodium_multi_current_o::NTBP_patlak_sodium_multi_current_o(const NTBP_patlak_sodium_multi_current_o & original)
        :
        NTBP_multi_current_o(original._reversalPotential(), original._density(), original._area(), original._conductivity())
{
    channelsPtr = new NTBP_patlak_sodium_ion_channels_o(  original._numChannels() );
    // add assignment code here
}

const NTBP_patlak_sodium_multi_current_o&
NTBP_patlak_sodium_multi_current_o::operator= (const NTBP_patlak_sodium_multi_current_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
// add assignment code here
    return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_patlak_sodium_multi_current_o::~NTBP_patlak_sodium_multi_current_o()
{
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
void
NTBP_patlak_sodium_multi_current_o::ComputeRateConstants(NTreal vM /* in mV */)
{
    alphaM=ForwardRate(vM,_temperature(),z_am,d_am,w_am);
    alphaM=alphaM*maxRate/(alphaM+maxRate);

    betaM=BackwardRate(vM,_temperature(),z_bm,d_bm,w_bm);
    betaM=betaM*maxRate/(betaM+maxRate);

    alphaH=ForwardRate(vM,_temperature(),z_ah,d_ah,w_ah);
    alphaH=alphaH*maxRate/(alphaH+maxRate);

    betaH=BackwardRate(vM,_temperature(),z_bh,d_bh,w_bh);
    betaH=betaH*maxRate/(betaH+maxRate);

    gamma=Gamma(vM,_temperature(),z_g,d_g,del_g,w_g);
    gamma=gamma*maxRate/(gamma+maxRate);

    delta=Delta(vM,_temperature(),z_d,d_d,del_d,w_d);
    delta=delta*maxRate/(delta+maxRate);

    kci=ForwardRate(vM,_temperature(),z_kci,d_kci,w_kci);
    kci=kci*maxRate/(kci+maxRate);

    kic=BackwardRate(vM,_temperature(),z_kic,d_kic,w_kic);
    kic=kic*maxRate/(kic+maxRate);

    kappa=Kappa(vM,_temperature(),z_k,d_k,del_k,w_k);
    kappa=kappa*maxRate/(kappa+maxRate);

    lambda=Delta(vM,_temperature(),z_l,d_l,del_l,w_l);
    lambda=lambda*maxRate/(lambda+maxRate);
}

/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
inline NTreturn NTBP_patlak_sodium_multi_current_o::StepCurrent()
{
//	cerr << "NTBP_patlak_sodium_multi_current_o::StepCurrent()" << endl;
    switch (_simulationMode()) {
    case NTBP_BINOMIALPOPULATION:
    {
        vector < NTreal > vec(10);
        vec[0] = alphaM;
        vec[1] = betaM;
        vec[2] = alphaH;
        vec[3] = betaH;
        vec[4] = gamma;
        vec[5] = delta;
        vec[6] = kic;
        vec[7] = kci;
        vec[8] = kappa;
        vec[9] = lambda;
        NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
        return (channelsPtr->BinomialStep());
    }
    break;
    case NTBP_SINGLECHANNEL:
    {
        vector < NTreal > vec(10);
        vec[0] = alphaM;
        vec[1] = betaM;
        vec[2] = alphaH;
        vec[3] = betaH;
        vec[4] = gamma;
        vec[5] = delta;
        vec[6] = kic;
        vec[7] = kci;
        vec[8] = kappa;
        vec[9] = lambda;
        NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );
        return (channelsPtr->Step());
    }
    break;
    default:
        cerr << "NTBP_patlak_sodium_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode." << endl;
        return NT_PARAM_UNSUPPORTED;
        break;
    }
    return NT_FAIL;
}

inline NTreal NTBP_patlak_sodium_multi_current_o::ComputeConductance()
{
//  cerr << "NTBP_patlak_sodium_multi_current_o::ComputeConductance" << endl;
    switch (_simulationMode()) {
    case NTBP_BINOMIALPOPULATION:
    case NTBP_SINGLECHANNEL:
        return Set_conductance( channelsPtr->NumOpen() * conductivity);
        break;
    default:
        cerr << "NTBP_patlak_sodium_multi_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance." << endl;
        return 0;
    }
}

/**  */
/** No descriptions */
inline NTreal
NTBP_patlak_sodium_multi_current_o::OpenChannels() const
{
    switch (_simulationMode()) {
    case NTBP_BINOMIALPOPULATION:
    case NTBP_SINGLECHANNEL:
        return channelsPtr->NumOpen();
        break;
    default:
        cerr << "NTBP_patlak_sodium_multi_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels." << endl;
        return 0;
    }
}

inline NTreal
NTBP_patlak_sodium_multi_current_o::ClosedChannels() const
{
    switch (_simulationMode()) {
    case NTBP_BINOMIALPOPULATION:
    case NTBP_SINGLECHANNEL:
        return channelsPtr->NumClosed();
        break;
    default:
        cerr << "NTBP_patlak_sodium_multi_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels." << endl;
        return 0;
    }
}





/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown

void
NTBP_patlak_sodium_multi_current_o::() const
{
}
 */


/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
