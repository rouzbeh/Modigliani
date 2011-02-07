/**\file ntbp_colbert_axonal_sodium_multi_current_obj.h - NTBP_colbert_axonal_sodium_multi_current_o class header
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

/* $Id: ntbp_colbert_axonal_sodium_multi_current_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: ntbp_colbert_axonal_sodium_multi_current_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.3  2003/04/08 15:13:36  face
 * *** empty log message ***
 *
 * Revision 1.2  2003/02/17 10:20:22  face
 * *** empty log message ***
 *
 * Revision 1.1  2003/01/30 17:13:42  face
 * *** empty log message ***
 *
 */

#ifndef _ntbp_colbert_axonal_sodium_multi_current_obj_h_
#define _ntbp_colbert_axonal_sodium_multi_current_obj_h_

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

/** @short NTBP_colbert_axonal_sodium_multi_current_o class
 Layer 5 pyramidal cell axonal (past initial segment) currents
 from Sprague-Dawley rat neocortical slices
 based on
 C.M. Colbert nad E. Pan(1995)
 "Ion channel properties underlying axonal action
 potential initiation in pyramidal neurons"
 Nature Neurosci., Vol. 5, No. 6., p.533-538

 \bug unknown
 \warning unknown
 */
class NTBP_colbert_axonal_sodium_multi_current_o: public NTBP_multi_current_o {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NTBP_colbert_axonal_sodium_multi_current_o(NTreal area /* in muMeter^2 */,
			NTreal density = 55 /* in num/muMeter^2 */,
			NTreal channelConductance = 14.8e-9, /* in mSiemens per channel = 14.8 at 24 C (Colbert & Johnston (1996))*/
			NTreal vBase = -63/* in mV */, NTreal newQ10m = 2.2,
			NTreal newQ10h = 2.9, NTreal reversalPotential = 118);
	NTBP_colbert_axonal_sodium_multi_current_o(
			const NTBP_colbert_axonal_sodium_multi_current_o & original);
	const NTBP_colbert_axonal_sodium_multi_current_o & operator=(
			const NTBP_colbert_axonal_sodium_multi_current_o & right);
	virtual ~NTBP_colbert_axonal_sodium_multi_current_o();
	/* ***  Methods              ***/
	/** in 1/ms or kHz */
	NTreal ComputeChannelStateTimeConstant() const;
	NTreturn StepCurrent();
	NTreal OpenChannels() const;
	NTreal ComputeConductance();
	void ComputeRateConstants(NTreal vM /* in mV */);
	void ShowChannelStatePopulation() {
		channelsPtr->ShowStates();
	}
	void Show() {
		cout << " alphaM= " << alphaM << "\t betaM= " << betaM
				<< "\t openRatio= " << ((double) channelsPtr->NumOpen())
				/ channelsPtr->_numChannels() << "\t alphaH= " << alphaH
				<< "\t betaH= " << betaH << "\t openNum= "
				<< channelsPtr->NumOpen();
	}
	/** Rate constants based on measurment data (Fig.2), not on model data */
	NTreal AlphaM(NTreal vM /* in mV */) {
		vM += _vBase();
		return 0.182 * (vM + 46.5) / (1.0 - exp((-vM - 46.5) / 6.0));
	}
	/** Rate constants based on measurment data (Fig.2), not on model data */
	NTreal BetaM(NTreal vM /* in mV */) {
		vM += _vBase();
		return 0.124 * (-vM - 46.5) / (1.0 - exp((vM + 46.5) / 6.0));
	}
	/** Rate constants based on measurment data (Fig.2), not on model data */
	NTreal AlphaH(NTreal vM /* in mV */) {
		vM += _vBase();
		return -0.015 * (vM + 69.0) / (1.0 - exp((vM + 69.0) / 6.0));
	}
	/** Rate constants based on measurment data (Fig.2), not on model data */
	NTreal BetaH(NTreal vM /* in mV */) {
		vM += _vBase();
		return -0.015 * (-vM - 69.0) / (1.0 - exp((-vM - 69.0) / 6.0));
	}
	/**  */
	void ShowParam() const;
	/* ***  Data                 ***/
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
	NTreal q10m;

	NTreal alphaH;
	NTreal betaH;
	NTreal h;
	NTreal noiseH;
	static NTreal alphaHvec[15000];
	static NTreal betaHvec[15000];
	NTreal q10h;
};

#endif /* _ntbp_colbert_axonal_sodium_multi_current_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
