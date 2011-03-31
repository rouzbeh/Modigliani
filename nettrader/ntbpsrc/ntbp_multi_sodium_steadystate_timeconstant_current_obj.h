/**\file ntbp_multi_sodium_steadystate_timeconstant_current_obj.h - NTBP_multi_sodium_steadystate_timeconstant_current_o class header 
 * by Ahmed Aldo Faisal &copy; created 3.7.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.0
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

/* $Id:$ 
 * $Log:$

 */
#ifndef _ntbp_multi_sodium_steadystate_timeconstant_current_obj_h_ 
#define _ntbp_multi_sodium_steadystate_timeconstant_current_obj_h_ 

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

/** @short NTBP_multi_sodium_steadystate_timeconstant_current_o class 

 This class of Na channel models is based upon the following structure of published data:

 m_\infty (V) , \tau_m(V), h_\infty(V), \tau_h(V) are given empirically.

 The following structures are defined:
 m_\infty(V)=1/(1+exp( -(V-V_{Moffset})/bM )) (FERMI)
 \tau_m(V) = cM (CONSTANT)

 h_\infty(V)=1/(1+exp( -(V-V_{Hoffset})/bH ) (FERMI)
 \tau_h(V) = aH exp( (V-V_{offset})^2 /cH ) (GAUSSIAN)

 \bug unknown
 \warning unknown
 */

class NTBP_multi_sodium_steadystate_timeconstant_current_o: public NTBP_multi_current_o {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NTBP_multi_sodium_steadystate_timeconstant_current_o(
			NTreal area /* in muMeter^2 */,
			NTreal density/* in num/muMeter^2 */,
			NTreal channelConductancse /* in mSiemens per channel */,
			NTBPKineticFunctionType newmKineticFnct,
			NTreal newvMOffset /* [mV] */, NTreal newaM, NTreal newbM,
			NTreal newcM, NTBPKineticFunctionType newmTKineticFnct,
			NTreal newvMTOffset /* [mV] */, NTreal newaMT, NTreal newbMT,
			NTreal newcMT, NTBPKineticFunctionType newhKineticFnct,
			NTreal newvHOffset /* [mV] */, NTreal newaH, NTreal newbH,
			NTreal newcH, NTBPKineticFunctionType newhTKineticFnct,
			NTreal newvHTOffset /* [mV] */, NTreal newaHT, NTreal newbHT,
			NTreal newcHT);
			NTBP_multi_sodium_steadystate_timeconstant_current_o(
					const NTBP_multi_sodium_steadystate_timeconstant_current_o & original);
	const NTBP_multi_sodium_steadystate_timeconstant_current_o & operator=(
			const NTBP_multi_sodium_steadystate_timeconstant_current_o & right);
	virtual ~NTBP_multi_sodium_steadystate_timeconstant_current_o();
	/* ***  Methods              ***/
	NTreal AlphaM(NTreal vM /* in mV */) {
		vM += _vBase();
		return mInfty(vM) / tauM(vM);
	}
	NTreal BetaM(NTreal vM /* in mV */) {
		vM += _vBase();
		return (1.0 - mInfty(vM)) / tauM(vM);
	}
	NTreal AlphaH(NTreal vM /* in mV */) {
		vM += _vBase();
		return hInfty(vM) / tauH(vM);
	}
	NTreal BetaH(NTreal vM /* in mV */) {
		vM += _vBase();
		return (1.0 - hInfty(vM)) / tauH(vM);
	}
	void ShowParam() const;
	//void Show() {  cout << " alphaM= " << alphaM << "\t betaM= " << betaM << "\t openRatio= " << //((double)channelsPtr->NumOpen())/channelsPtr->_numChannels()
	//				   << "\t alphaH= " << alphaH << "\t betaH= " << betaH << "\t openNum= " << channelsPtr->NumOpen();}
	void Show() {
		cout << "\t" << alphaM << "\t" << betaM << "\t"
				<< ((double) channelsPtr->NumOpen())
						/ channelsPtr->_numChannels() << "\t" << alphaH << "\t"
				<< betaH << "\t" << channelsPtr->NumOpen();
	}
	/* ***  Data                 ***/
	static NTBP_transition_rate_matrix_o* probMatrix;

protected:
	/* ***  Methods              ***/
	/** in 1/ms or kHz */
	NTreal ComputeChannelStateTimeConstant() const;
	NTreturn StepCurrent();
	NTreal OpenChannels() const;
	NTreal ComputeConductance();
	void ComputeRateConstants();
	void ShowChannelStatePopulation() {
		channelsPtr->ShowStates();
	}
	//NOTE: IF FermiTemperature positive then BottomLeft to TopRight SigmoidAlphaH
	// SET CURRECT SIGN BY SETTING positive or negative (e.g. positive for alpha
	// and negative for beta
	inline NTreal Fermi(NTreal x, NTreal offset, NTreal temp) {
		return 1.0 / (1.0 + exp(-(x - offset) / temp));
	}

	inline NTreal Gaussian(NTreal x, NTreal offset, NTreal sig) {
		NTreal tmpDiff = (x - offset);
		NTreal tmpResult = exp(-0.5 * tmpDiff * tmpDiff / (sig * sig));
		//cout <<"Gaussian " << tmpResult << " x" << x << " x-offset=" << tmpDiff <<" sig= " << sig << endl;
		return tmpResult;
	}

	inline NTreal mInfty(NTreal vM) {
		return Fermi(vM, vMOffset, bM);
	}
	inline NTreal tauM(NTreal vM) {
		return cMT;
	}
	inline NTreal hInfty(NTreal vM) {
		return Fermi(vM, vHOffset, bH);
	}
	inline NTreal tauH(NTreal vM) {
		NTreal tmp = aHT * Gaussian(vM, vHTOffset, cHT);
		//cout << "tauH("<<vM<<")="<<tmp<< " " << aHT << endl;
		return tmp;
	}
	/* ***  Data                 ***/
	NTreal vMOffset; // [mV]
	NTreal aM; //
	NTreal bM; // "Slope" [mV]
	NTreal cM; // time constant \tau_\infty [mS]
	NTreal vMTOffset; // [mV]
	NTreal aMT; //
	NTreal bMT; // "Slope" [mV]
	NTreal cMT; // time constant \tau_\infty [mS]
	NTreal vHOffset; // [mV]
	NTreal aH; // scale
	NTreal bH; // "Slope" [mV]
	NTreal cH; // "width" [mV^2] !
	NTreal vHTOffset; // [mV]
	NTreal aHT; // scale
	NTreal bHT; // "Slope" [mV]
	NTreal cHT; // "width" [mV^2] !

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
#endif /* _ntbp_multi_sodium_steadystate_timeconstant_current_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
