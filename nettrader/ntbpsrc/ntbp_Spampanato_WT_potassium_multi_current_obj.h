


#ifndef _ntbp_Spampanato_WT_potassium_multi_current_obj_h_
#define _ntbp_Spampanato_WT_potassium_multi_current_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* Parent includes */
#include "ntbp_multi_current_obj.h"
/* NT includes */
//#include "ntbp_potassium_ion_channels_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"
/* other includes */
#include <cmath>

/** @short NTBP_Spampanato_WT_potassium_multi_current_o class
based on
Spampanato 2004a:
Increased neural firing in computer simulations of potassium channel
mutations that cause generalized epilepsy with febrile seizures plus.
and
Aradi2002
and
Yuen1991

\bug unknown
\warning unknown
*/
class NTBP_Spampanato_WT_potassium_multi_current_o : public NTBP_multi_current_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/
	//all standard values removed to ensure constant conditions
NTBP_Spampanato_WT_potassium_multi_current_o(NTreal area /* in muMeter^2 */,
										 NTreal density /* in num/muMeter^2 */,
										 NTreal channelConductance, /* in mSiemens per channel = 13pS */
										 NTreal vBase, /* in mV */
										 NTreal newQ10n ,
										 NTreal reversalPotential ,
										 NTreal timeStep ,
										 NTreal newTemperature );

NTBP_Spampanato_WT_potassium_multi_current_o(const NTBP_Spampanato_WT_potassium_multi_current_o & original);
const NTBP_Spampanato_WT_potassium_multi_current_o & operator= (const NTBP_Spampanato_WT_potassium_multi_current_o & right);
virtual ~NTBP_Spampanato_WT_potassium_multi_current_o();
virtual NTreturn Set_q10(NTreal newQ10) {
	cerr << "WARNING : Set_q10 is being called on Spampanato WT potassium channel. NOT IMPLEMENTED.";
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
void Show() {  cout << " alphaN= " << AlphaN(voltage) << "\t betaN= " << BetaN(voltage)	<< "\t openRatio= "
	<< ((double)channelsPtr->NumOpen())/channelsPtr->_numChannels() << "\t openNum= " << channelsPtr->NumOpen();}

//"exact version" of rates
NTreal AlphaN(NTreal vM /* in mV */) { 	 vM += _vBase(); return -0.07*(vM-47)/(1-exp((vM-47)/-6));}
NTreal BetaN(NTreal vM /* in mV */) { 	vM += _vBase(); return 0.264*exp((vM-22)/4); }


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
NTreal alphaN;
NTreal betaN;
NTreal n;
NTreal noiseN;
static NTreal alphaNvec[15000];
static NTreal betaNvec[15000];
const NTreal q10n;
static NTreal nvec[15000];
};


#endif /* _ntbp_Spampanato_WT_potassium_multi_current_obj_h_ */
