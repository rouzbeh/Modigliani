/**\fil-e ntbp_HH_multi_sodium_current_obj.h - NTBP_HH_multi_sodium_current_o class header
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

/* $Id: ntbp_HH_multi_sodium_current_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: ntbp_HH_multi_sodium_current_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.3  2003/01/17 16:53:13  face
 * *** empty log message ***
 *
 * Revision 1.2  2002/03/13 19:23:27  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#ifndef _ntbp_HH_multi_sodium_current_obj_h_
#define _ntbp_HH_multi_sodium_current_obj_h_

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* Parent includes */
#include "ntbp_multi_current_obj.h"
/* NT includes */
#include "ntbp_sodium_ion_channels_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"
/* other includes */
#include <cmath>

/** @short NTBP_HH_multi_sodium_current_o class
 Channel density 330/muM^2
 Single channel conductivity 4 pS
 Temperature 6.3 Celsius
 (see Hille (1992) p. 329 or Conti et. al (1975) )
 \bug unknown
 \warning unknown
 */
class NTBP_HH_multi_sodium_current_o: public NTBP_multi_current_o {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NTBP_HH_multi_sodium_current_o(NTreal area /* in muMeter^2 */,
			NTreal density = 120 /* in num/muMeter^2 */,
			NTreal channelConductance = 2.0e-8, /* in mSiemens per channel = 13pS */
			NTreal vBase = 0, /* in mV */
			NTreal newQ10m = 3, NTreal newQ10h = 3, NTreal reversalPotential =
					115, NTreal timeStep = 0.001, NTreal newTemperature = 6.3);
	NTBP_HH_multi_sodium_current_o(
			const NTBP_HH_multi_sodium_current_o & original);
	const NTBP_HH_multi_sodium_current_o & operator=(
			const NTBP_HH_multi_sodium_current_o & right);
	virtual ~NTBP_HH_multi_sodium_current_o();
	virtual NTreturn Set_q10(NTreal newQ10) {
		cerr << "WARNING : Set_q10 is being called on a multi sodium channel.";
		return NT_NOT_IMPLEMENTED;
	}
	/* ***  Methods              ***/
	/** in 1/ms or kHz */
	NTreal ComputeChannelStateTimeConstant() const;
	NTreturn StepCurrent();
	NTreal OpenChannels() const;
	NTreal OpenChannelsRatio() const;
	NTreal ComputeConductance();
	void ComputeRateConstants();
	void ShowChannelStatePopulation() {
		channelsPtr->ShowStates();
	}
	void Show() {
		cout << " alphaM= " << AlphaM(voltage) << "\t betaM= "
				<< BetaM(voltage) << "\t openRatio= "
				<< ((double) channelsPtr->NumOpen())
						/ channelsPtr->_numChannels() << "\t alphaH= "
				<< AlphaH(voltage) << "\t betaH= " << BetaH(voltage)
				<< "\t openNum= " << channelsPtr->NumOpen();
	}
	NTreal AlphaM(NTreal vM /* in mV */) {
		vM += _vBase();
		return (25.0 - vM) / (10.0 * (exp((25.0 - vM) / 10.0) - 1.0));
	}
	NTreal BetaM(NTreal vM /* in mV */) {
		vM += _vBase();
		return 4.0 * exp(-vM / 18.0);
	}
	NTreal AlphaH(NTreal vM /* in mV */) {
		vM += _vBase();
		return 0.07 * exp(-vM / 20.0);
	}
	NTreal BetaH(NTreal vM /* in mV */) {
		vM += _vBase();
		return 1.0 / (exp(3.0 - vM / 10.0) + 1.0);
	}
	/**  */
	void ShowParam() const;
	/* ***  Data                 ***/
	static NTBP_transition_rate_matrix_o* probMatrix;

protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	static NT_gaussian_rnd_dist_o normalRnd; // notice, sometimes a linker error is produced, which can be removed by adding a NT_gaussian_..._o object at the beginning of the main() code.

	static bool initTableLookUp;

	NTreal baseTemp; // C
	NTreal alphaM;
	NTreal betaM;
	NTreal m;
	NTreal noiseM;
	static NTreal alphaMvec[15000];
	static NTreal betaMvec[15000];
	const NTreal q10m;

	NTreal alphaH;
	NTreal betaH;
	NTreal h;
	NTreal noiseH;
	static NTreal alphaHvec[15000];
	static NTreal betaHvec[15000];
	static NTreal mvec[15000];
	static NTreal hvec[15000];
	const NTreal q10h;
};

#endif /* _ntbp_HH_multi_sodium_current_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
