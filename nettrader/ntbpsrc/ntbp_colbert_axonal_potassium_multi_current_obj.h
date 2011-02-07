/**\file ntbp_colbert_axonal_potassium_multi_current_obj.h - NTBP_colbert_axonal_potassium_multi_current_o class header
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

/* $Id: ntbp_colbert_axonal_potassium_multi_current_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: ntbp_colbert_axonal_potassium_multi_current_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.4  2003/08/24 10:43:01  face
 * *** empty log message ***
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
 * Revision 1.2  2002/03/13 19:23:27  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#ifndef _ntbp_colbert_axonal_potassium_multi_current_obj_h_
#define _ntbp_colbert_axonal_potassium_multi_current_obj_h_

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

/** @short NTBP_colbert_axonal_potassium_multi_current_o class */
/*
 Delayed rectifier potassium current based on
 axonal simulation
 C.M. Colbert nad E. Pan (1995)
 "Ion channel properties underlying axonal action
 potential initiation in pyramidal neurons"
 Nature Neurosci., Vol. 5, No. 6., p.533-538
 with kinetics from
 Hoffman, D. A., Magee, J. C., Colbert, C. M. & Johnston, D. (1997)
 "K+ channel regulation of signal propagation in
 dendrites of hippocampal pyramidal neurons"
 Nature, Vol. 387, p. 869-875

 The original potassium's n-gate is not fully specified by
 the alpha and beta rate constants but
 by a voltage independent time constant tau_n = 1.8 ms.
 (i.e. not 1/(\alpha_n + \beta_n))
 My Mathematica modelling shows that an equivalent
 kinetic model exists which reproduces perfectly the
 observed gating without the need of constants and
 thus allows for stochastic modelling.

 \warning
 */

// MAY NOT BE WORKING PROPERLY - RECHECK!
// SINCE RESTING POTENTIAL IS NOT WELL ESTABLISHED
class NTBP_colbert_axonal_potassium_multi_current_o: public NTBP_multi_current_o {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NTBP_colbert_axonal_potassium_multi_current_o(
			NTreal area /* in muMeter^2 */,
			NTreal density = 8/* num/muMeter^2 */, NTreal channelConductance =
					2.0e-8, /* in mSiemens per channel = 13pS */
			NTreal newVBase = -64.5 /*m mV */, NTreal newQ10 = 3,
			NTreal reversalPotential = -15.5);
	NTBP_colbert_axonal_potassium_multi_current_o(
			const NTBP_colbert_axonal_potassium_multi_current_o & original);
	const NTBP_colbert_axonal_potassium_multi_current_o & operator=(
			const NTBP_colbert_axonal_potassium_multi_current_o & right);
	virtual ~NTBP_colbert_axonal_potassium_multi_current_o();
	/* ***  Methods              ***/
	NTreal ComputeRateSum() const;
	NTreal OpenChannels() const;
	NTreal ComputeConductance();
	void ComputeRateConstants(NTreal voltage);
	/**  in kHz */
	NTreal ComputeChannelStateTimeConstant() const;
	/**  */

	NTreturn StepCurrent();
	/** rate constant in ms^-1 */
	NTreal AlphaN(NTreal vM /* in mV */) {
		vM += _vBase();
		return 0.555556 / (1.0 + exp((-30.0 - vM) / 13.0));
	}
	NTreal BetaN(NTreal vM /* in mV */) {
		vM += _vBase();
		return 0.555556 / (1.0 + exp((+30.0 + vM) / 13.0));
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
	NTBPdelayedPotassiumRectifierType type;
	static NT_gaussian_rnd_dist_o normalRnd;
	static bool initTableLookUp;
	static NTreal alphaNvec[15000];
	static NTreal betaNvec[15000];
	NTreal q10;
	NTreal alphaN;
	NTreal betaN;
	NTreal n;
	NTreal noiseN;
	NTreal baseTemp;
};
#endif /* _ntbp_colbert_axonal_potassium_multi_current_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
