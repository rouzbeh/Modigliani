/**\file ntbp_multi_current_obj.h - NTBP_multi_current_o class header
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


/* $Id: ntbp_multi_current_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
* $Log: ntbp_multi_current_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.5  2003/01/30 17:13:43  face
* *** empty log message ***
*
* Revision 1.4  2003/01/17 16:53:13  face
* *** empty log message ***
*
* Revision 1.3  2002/03/13 19:23:27  face
* *** empty log message ***
*
* Revision 1.2  2001/10/12 09:18:36  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_multi_current_obj_h_
#define _ntbp_multi_current_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* Parent includes */
#include "ntbp_membrane_current_obj.h"
/* NT includes */
#include "ntbp_ion_channels_obj.h"
/* other includes */

/** @short NTBP_multi_current_o class
 	Base class for stochastic channel simulations of individual channels
\bug unknown
\warning unknown
*/
class NTBP_multi_current_o : public NTBP_membrane_current_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/
NTBP_multi_current_o(
	NTreal reversalPotential, // in mV
	NTreal density, // channels per mumeter^2
	NTreal area, // in mumeter^2
	NTreal conductivity, // in mSiemens per channel
	NTreal vBase = 0 // mV
	);
NTBP_multi_current_o(const NTBP_multi_current_o & original);
const NTBP_multi_current_o & operator= (const NTBP_multi_current_o & right);
virtual ~NTBP_multi_current_o();
/* ***  Methods              ***/
//virtual NTreal ComputeConductance() = 0;
//virtual NTreturn StepCurrent() = 0;
/* mementary total conductance */
NTreal _density() const { return density;}
NTreal _area()  const { return area; }
/* conductivity per channel in mSiemens */
NTreal _conductivity() const { return conductivity; }
/* conductivity if all channels open in mSiemens/cm^2, */
NTreal _maxConductivity() const {return density /* num/muMeter^2 */ * conductivity /* mSiemens */* 1.0e8 /* muMeter^2/cm^2 */;}
 /**  */
bool ComputeGillespieStep() {cerr <<"NTBP_multi_current_o::ComputeGillespieStep()" << endl; return channelsPtr->GillespieStep();}
void UpdateNumChannels() { numChannels = (NTsize) ceil(density*area); }
NTsize _numChannels() const { return numChannels; }
/** Number of total ionic channels */
NTreal NumChannels() const { return _numChannels(); }
NTreal OpenChannelsRatio() const { return OpenChannels()/NumChannels(); }
NTreal _vBase() const { return vBase;}
// Dangerous: since cached values are not automatically recomputed in derived classes
//void Set_vBase(NTreal newVBase /* mV */){ vBase = newVBase; }
/* ***  Data                 ***/
protected:
/* ***  Methods              ***/
/* ***  Data                 ***/
static NT_uniform_rnd_dist_o uniformRnd;
NTreal conductivity; // in mSiemens per channel
NTreal density; // channels per muMeter^2
NTreal vBase; // mV
NTBP_ion_channels_o * channelsPtr;
private:
/* ***  Methods              ***/
/* ***  Data                 ***/
NTsize numChannels;
NTreal area; // in mumeter^2

};

#endif /* _ntbp_multi_current_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
