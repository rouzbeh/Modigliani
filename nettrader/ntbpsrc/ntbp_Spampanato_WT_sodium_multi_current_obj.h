


#ifndef _ntbp_Spampanato_WT_sodium_multi_current_obj_h_
#define _ntbp_Spampanato_WT_sodium_multi_current_obj_h_

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

/** @short NTBP_Spampanato_WT_sodium_multi_current_o class
based on
Spampanato 2004a:
Increased neural firing in computer simulations of sodium channel
mutations that cause generalized epilepsy with febrile seizures plus

\bug unknown
\warning unknown
*/
class NTBP_Spampanato_WT_sodium_multi_current_o : public NTBP_multi_current_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/
	//all standard values removed to ensure constant conditions
NTBP_Spampanato_WT_sodium_multi_current_o(NTreal area /* in muMeter^2 */,
										 NTreal density /* in num/muMeter^2 */,
										 NTreal channelConductance, /* in mSiemens per channel = 13pS */
										 NTreal vBase, /* in mV */
										 NTreal newQ10m ,
										 NTreal newQ10h ,
										 NTreal reversalPotential ,
										 NTreal timeStep ,
										 NTreal newTemperature );

NTBP_Spampanato_WT_sodium_multi_current_o(const NTBP_Spampanato_WT_sodium_multi_current_o & original);
const NTBP_Spampanato_WT_sodium_multi_current_o & operator= (const NTBP_Spampanato_WT_sodium_multi_current_o & right);
virtual ~NTBP_Spampanato_WT_sodium_multi_current_o();
virtual NTreturn Set_q10(NTreal newQ10) {
	cerr << "WARNING : Set_q10 is being called on Spampanato WT sodium channel. NOT IMPLEMENTED.";
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
void ShowChannelStatePopulation() { channelsPtr->ShowStates(); }
void Show() {  cout << " alphaM= " << AlphaM(voltage) << "\t betaM= " << BetaM(voltage) << "\t openRatio= " << ((double)channelsPtr->NumOpen())/channelsPtr->_numChannels()
				   << "\t alphaH= " << AlphaH(voltage) << "\t betaH= " << BetaH(voltage) << "\t openNum= " << channelsPtr->NumOpen();}

//"exact version" of rates
NTreal AlphaM(NTreal vM /* in mV */) { 	 vM += _vBase(); return 20/(3*(1/exp( 0.2041*vM + 4.3265) + 1));}
NTreal BetaM(NTreal vM /* in mV */) { 	vM += _vBase(); return 6.6667 - 20/(3*(1/exp(0.2041*vM + 4.3265) + 1)); }
NTreal AlphaH(NTreal vM /* in mV */) { 	vM += _vBase(); return (10*exp(pow((0.0306*vM +  1.8777),2)/2))/(201*(exp(0.1299*vM +  5.1558) + 1)); }
NTreal BetaH(NTreal vM /* in mV */) { 	vM += _vBase(); return -(10*exp(pow((0.0306*vM +  1.8777),2)/2)*(1/(exp(0.1299*vM +  5.1558) + 1) - 1))/201; }

//HH-rates
//NTreal AlphaM(NTreal vM /* in mV */) { 	 vM += _vBase(); return 1.86*(vM+18.4)/(1-exp((-18.4-vM)/10.3));}
//NTreal BetaM(NTreal vM /* in mV */) { 	vM += _vBase(); return 0.0860*(-22.7-vM)/(1-exp((vM+22.7)/9.16)); }
//NTreal AlphaH(NTreal vM /* in mV */) { 	vM += _vBase(); return 0.0336*(-111.0-vM)/(1-exp((vM+111.0)/11.0)); }
//NTreal BetaH(NTreal vM /* in mV */) { 	vM += _vBase(); return 2.30/(1+exp((-28.8-vM)/13.4)); }

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


#endif /* _ntbp_Spampanato_WT_sodium_multi_current_obj_h_ */
