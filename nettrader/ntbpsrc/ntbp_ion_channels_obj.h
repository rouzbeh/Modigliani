/**\file ntbp_ion_channels_obj.h - NTBP_ion_channels_o class header 
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
  

/* $Id: ntbp_ion_channels_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_ion_channels_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
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

#ifndef _ntbp_ion_channels_obj_h_
#define _ntbp_ion_channels_obj_h_ 

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* Parent includes */
#include "ntbp_object_obj.h" 
/* NT includes */
#include "nt_error_obj.h"
#include "nt_uniform_rnd_dist_obj.h"
#include "nt_binomial_rnd_dist_obj.h"
/* other includes */
#include <vector>
#include <iostream>

/** @short NTBP_ion_channels_o class 
\bug unknown
\warning unknown 
*/
class NTBP_ion_channels_o : public NTBP_object_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NTBP_ion_channels_o(NTsize numNewChannels = 1, NTsize numNewStates = 1);
NTBP_ion_channels_o(const NTBP_ion_channels_o & original);
const NTBP_ion_channels_o & operator= (const NTBP_ion_channels_o & right);
virtual ~NTBP_ion_channels_o();
/* ***  Methods              ***/
NTsize _numChannels() const { return numChannels;}
NTsize _numStates() const { return numStates;}
bool GillespieStep();
virtual NTreturn BinomialStep() { cerr << "NTBP_ion_channels_o::BinomialStep was not derived." << endl; return NT_NOT_IMPLEMENTED;}
virtual bool ComputeGillespieStep(NTsize channelStateId){ cerr << "NTBP_ion_channels::ComputeGillespieStep - Error : Should be overridden by derived class." << endl; return false;}
virtual void ShowStates() const = 0;
virtual NTsize NumOpen() const = 0;
virtual NTsize NumClosed() const = 0;
virtual NTreturn SteadyStateDistribution() = 0;
/** Sum of escape rates from current state [1/s] */
virtual NTreal ComputeChannelStateTimeConstant() const = 0;
virtual NTreturn UpdateStateProb(vector <NTreal> rateConstVec /* in mSec^-1 */) = 0;
  /**  */
/* ***  Data                 ***/
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/
vector < NTreal > statePersistenceProbVec;
vector < NTsize > stateCounterVec;
static NT_uniform_rnd_dist_o uniformRnd; // [0,1] randum number generation
static NT_binomial_rnd_dist_o binomRnd;
private:
/* ***  Methods              ***/  
/* ***  Data                 ***/
NTsize numStates;
NTsize numChannels;
};

#endif /* _ntbp_ion_channels_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
