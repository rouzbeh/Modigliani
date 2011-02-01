/**\file ntbp_hranvier_potassium_multi_current_obj.h - NTBP_hranvier_potassium_multi_current_o class header
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


/* $Id: ntbp_hranvier_potassium_multi_current_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
* $Log: ntbp_hranvier_potassium_multi_current_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2003/01/30 17:13:42  face
* *** empty log message ***
*
* Revision 1.2  2002/03/13 19:23:27  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_hranvier_potassium_multi_current_obj_h_
#define _ntbp_hranvier_potassium_multi_current_obj_h_


/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* Parent includes */
#include "ntbp_multi_current_obj.h"
/* NT includes */
#include "ntbp_potassium_ion_channels_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"
/* other includes */


/** @short NTBP_hranvier_potassium_multi_current_o class */
/*
Fast potassium current based on:
Schwartz, J. and Reid, G. and Bostock, H. (1995)
"Action potentials and membrane currents in the human node of Ranvier"
J.Physiol. 430:283-292

\bug unknown
\warning unknown
*/


class NTBP_hranvier_potassium_multi_current_o : public NTBP_multi_current_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/
NTBP_hranvier_potassium_multi_current_o(NTreal area /* in muMeter^2 */,
																				NTreal density /* num/muMeter^2 */,
																				NTreal channelConductance = 2.0e-8, /* in mSiemens per channel = 13pS */
																				NTreal newVBase = -84 /*m mV */,
																				NTreal q10 = 3,
																				NTreal reversalPotential = -15
  																	 		);
NTBP_hranvier_potassium_multi_current_o(const NTBP_hranvier_potassium_multi_current_o & original);
const NTBP_hranvier_potassium_multi_current_o & operator= (const NTBP_hranvier_potassium_multi_current_o & right);
virtual ~NTBP_hranvier_potassium_multi_current_o();
/* ***  Methods              ***/
NTreal ComputeRateSum() const;
NTreal OpenChannels() const;
NTreal OpenChannelsRatio() const;
NTreal ComputeConductance();
void ComputeRateConstants(NTreal voltage);
  /**  in kHz */
NTreal ComputeChannelStateTimeConstant() const;
  /**  */

NTreturn StepCurrent();
/** rate constant in ms^-1 */
NTreal AlphaN( NTreal vM /* in mV */ ) { vM += _vBase(); return 0.00798*(vM+93.2)/(1-exp((-93.2-vM)/1.10));}
NTreal BetaN ( NTreal vM /* in mV */ ) { vM += _vBase(); return 0.0142*(-76.0-vM)/(1-exp((vM+76.0)/10.5));}
  /**  */
void ShowParam() const;
/* ***  Data                 ***/
protected:
/* ***  Methods              ***/
/* ***  Data                 ***/
private:
/* ***  Methods              ***/
/* ***  Data                 ***/
NTBPdelayedPotassiumRectifierType type;
static NT_gaussian_rnd_dist_o normalRnd;
static bool initTableLookUp;
static NTreal alphaNvec [15000];
static NTreal betaNvec [15000];
NTreal alphaN;
NTreal betaN;
NTreal n;
NTreal noiseN;
NTreal baseTemp;
NTreal q10;
};
#endif /* _ntbp_hranvier_potassium_multi_current_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
