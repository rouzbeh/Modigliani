/**\file ntbp_patlak_sodium_multi_current_obj.h - NTBP_patlak_sodium_multi_current_o class header 
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
  

/* $Id: ntbp_patlak_sodium_multi_current_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_patlak_sodium_multi_current_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2003/01/17 16:56:14  face
* *** empty log message ***
*

*/
#ifndef _ntbp_patlak_sodium_multi_current_obj_h_ 
#define _ntbp_patlak_sodium_multi_current_obj_h_ 

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* Parent includes */
#include "ntbp_multi_current_obj.h" 
/* NT includes */
#include "ntbp_patlak_sodium_ion_channels_obj.h"
/* other includes */

/** @short NTBP_patlak_sodium_multi_current_o class 
\bug unknown
\warning unknown 
*/
class NTBP_patlak_sodium_multi_current_o : public NTBP_multi_current_o {
public:
/***   Constructors, Copy/Assignment and D estructor  ***/
NTBP_patlak_sodium_multi_current_o(NTreal area /* in muMeter^2 */, NTreal density = 60 /* in num/muMeter^2 */, NTreal channelConductave = 2.0e-8 /* in mSiemens per channel = 20pS */);
NTBP_patlak_sodium_multi_current_o(const NTBP_patlak_sodium_multi_current_o & original);
const NTBP_patlak_sodium_multi_current_o & operator= (const NTBP_patlak_sodium_multi_current_o & right);
virtual ~NTBP_patlak_sodium_multi_current_o();
/* ***  Methods              ***/  
NTreturn StepCurrent();
NTreal OpenChannels() const;
NTreal ClosedChannels() const;
NTreal ComputeConductance();
void ComputeRateConstants(NTreal vM /* in mV */);
void ShowChannelStatePopulation() { channelsPtr->ShowStates(); }
void Show() {  cout << "alphaM=" << alphaM << "\tbetaM=" << betaM << "\topenRatio=" << ((double)channelsPtr->NumOpen())/channelsPtr->_numChannels()
				   << "\talphaH=" << alphaH << "\tbetaH=" << betaH << "\tgamma="<< gamma << "\tdelta="<<delta<< "\t kci="<<kci << "\t kic="<<kic<< "\tkappa="<<kappa<<"\tlambda="<<lambda<<endl;}

/** in ms^-1 */				
NTreal ForwardRate(NTreal vM /* mV */,
            NTreal temp /* Celsius */,
            NTreal z /* elementary charge */,
            NTreal d /* unit free */,
            NTreal w /* units of kB*T */)
{
              NTreal T = temp + NT_0_CELSIUS;// [Kelvin]
							NTreal v = vM/1000.0; // [V]
							NTreal kT = (NT_BOLTZMANN*T);					
							return (kT/NT_PLANCK)* exp(w + z*d*v*NT_PROTON_CHARGE/kT)/1000.0;
}

/** in ms^-1 */				
NTreal BackwardRate(NTreal vM /* mV */,
            NTreal temp /* Celsius */,
            NTreal z /* elementary charge */,
            NTreal d /* unit free */,
            NTreal w /* units of kB*T */)
{
              NTreal T = temp + NT_0_CELSIUS;//[K]
              NTreal v = vM/1000.0; // [V]
							NTreal kT = (NT_BOLTZMANN*T);
							return (kT/NT_PLANCK) * exp(w - z*(1-d)*v*NT_PROTON_CHARGE/kT)/1000.0;
}


NTreal Delta(NTreal vM /* mV */,
            NTreal temp /* Celsius */,
            NTreal z /* elementary charge */,
            NTreal d /* unit free */,
            NTreal del /* unit free */,
            NTreal w /* units of kB*T */)
{
              NTreal T = temp + NT_0_CELSIUS;//[K]
              NTreal v = vM/1000.0; // [V]
							NTreal kT = (NT_BOLTZMANN*T);
							return (kT/NT_PLANCK) * exp((w + d*del) + z*d*v*NT_PROTON_CHARGE/kT)/1000.0;
}


NTreal Gamma(NTreal vM /* mV */,
            NTreal temp /* Celsius */,
            NTreal z /* elementary charge */,
            NTreal d /* unit free */,
            NTreal del /* unit free */,
            NTreal w /* units of kB*T */)
{	
              NTreal T = temp + NT_0_CELSIUS;//[K]
              NTreal v = vM/1000.0; // [V]
							NTreal kT = (NT_BOLTZMANN*T);
							return (kT/NT_PLANCK) * exp((w + (1-d)*del) - z*(1-d)*v*NT_PROTON_CHARGE/kT)/1000.0;
}


NTreal Kappa(NTreal vM /* mV */,
            NTreal temp /* Celsius */,
            NTreal z /* elementary charge */,
            NTreal d /* unit free */,
            NTreal del /* unit free */,
            NTreal w /* units of kB*T */)
{	
              NTreal T = temp + NT_0_CELSIUS;//[K]
              NTreal v = vM/1000.0; // [V]
							NTreal kT = (NT_BOLTZMANN*T);
							return (kT/NT_PLANCK) * exp((w + (1-d)/del) + z*d*v*NT_PROTON_CHARGE/kT)/1000.0;
}


NTreal Lambda(NTreal vM /* mV */,
            NTreal temp /* Celsius */,
            NTreal z /* elementary charge */,
            NTreal d /* unit free */,
            NTreal del /* unit free */,
            NTreal w /* units of kB*T */)
{	
              NTreal T = temp + NT_0_CELSIUS;//[K]
              NTreal v = vM/1000.0; // [V]
							NTreal kT = (NT_BOLTZMANN*T);
							return (kT/NT_PLANCK) * exp((w + d*del) - z*(1-d)*v*NT_PROTON_CHARGE/kT)/1000.0;
}


/* ***  Data                 ***/
protected:
/* ***  Methods              ***/
/* ***  Data                 ***/
private:
/* ***  Methods              ***/
/* ***  Data                 ***/
// general
NTreal maxRate;
// \alpha_m
NTreal alphaM;
NTreal w_am;
NTreal z_am;
NTreal d_am;
// \beta_m
NTreal betaM;
NTreal w_bm;
NTreal z_bm;
NTreal d_bm;
// \alpha_h
NTreal alphaH;
NTreal w_ah;
NTreal z_ah;
NTreal d_ah;
// \beta_h
NTreal betaH;
NTreal w_bh;
NTreal z_bh;
NTreal d_bh;
// k_{ci}
NTreal kci;
NTreal w_kci;
NTreal z_kci;
NTreal d_kci;
// k_{ic}
NTreal kic;
NTreal w_kic;
NTreal z_kic;
NTreal d_kic;
// \delta
NTreal delta;
NTreal w_d;
NTreal del_d;
NTreal z_d;
NTreal d_d;
// \gamma
NTreal gamma;
NTreal w_g;
NTreal del_g;
NTreal z_g;
NTreal d_g;
// \kappa
NTreal kappa;
NTreal w_k;
NTreal del_k;
NTreal z_k;
NTreal d_k;
// \lambda
NTreal lambda;
NTreal w_l;
NTreal del_l;
NTreal z_l;
NTreal d_l;

};

#endif /* _ntbp_patlak_sodium_multi_current_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
