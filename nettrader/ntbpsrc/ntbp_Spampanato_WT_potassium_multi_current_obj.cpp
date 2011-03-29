
#include "ntbp_Spampanato_WT_potassium_multi_current_obj.h"

NT_gaussian_rnd_dist_o NTBP_Spampanato_WT_potassium_multi_current_o::normalRnd;
bool NTBP_Spampanato_WT_potassium_multi_current_o::initTableLookUp = false;
NTreal NTBP_Spampanato_WT_potassium_multi_current_o::alphaNvec[15000];
NTreal NTBP_Spampanato_WT_potassium_multi_current_o::betaNvec[15000];

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_Spampanato_WT_potassium_multi_current_o */
NTBP_Spampanato_WT_potassium_multi_current_o::NTBP_Spampanato_WT_potassium_multi_current_o(
		NTreal newArea, NTreal newDensity, NTreal newConductivity,
		NTreal newVBase, NTreal newQ10n,
		NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature) :
	q10n(newQ10n), NTBP_multi_current_o(
			reversalPotential /* in mV */, newDensity /* channels per mu^2 */,
			newArea /* in mu^2 */, newConductivity /* in mS per channel  */,
			newVBase /* mV */
	) {
	//density and area updated by NTBP_multi_current_obj constructor
	UpdateNumChannels();
	channelsPtr = new NTBP_ion_channels_o(_numChannels(), 5, newTimeStep);
	channelsPtr->setAsOpenState(5);
	//TODO: What are noiseM / -H?
	noiseN = 0;
	baseTemp = 20.0;	//all measurements at 20C
	setTimeStep(newTimeStep);
	Set_temperature(newTemperature);
	NTreal vTmp = 0;

	if (false == initTableLookUp) {
		for (NTsize ll = 0; ll < 15000; ll++) {
			vTmp = ((float) ll) / 100.0 - 20 + 0.005 /* otherwise we hit undefined points of alpha and beta function */;
			alphaNvec[ll] = AlphaN(vTmp);
			betaNvec[ll] = BetaN(vTmp);
		}
		initTableLookUp = true;
	}

	// this is n_inf
	n = AlphaN(0) / (AlphaN(0) + BetaN(0));
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_Spampanato_WT_potassium_multi_current_o::NTBP_Spampanato_WT_potassium_multi_current_o(
		const NTBP_Spampanato_WT_potassium_multi_current_o & original) :
	q10n(original.q10n), NTBP_multi_current_o(
			original._reversalPotential(), original._density(),
			original._area(), original._conductivity()) {
	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 5);
	channelsPtr->setAsOpenState(5);
}

const NTBP_Spampanato_WT_potassium_multi_current_o&
NTBP_Spampanato_WT_potassium_multi_current_o::operator=(
		const NTBP_Spampanato_WT_potassium_multi_current_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 5);
	channelsPtr->setAsOpenState(5);
	return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_Spampanato_WT_potassium_multi_current_o::~NTBP_Spampanato_WT_potassium_multi_current_o() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline NTreturn NTBP_Spampanato_WT_potassium_multi_current_o::StepCurrent() {
	//	cerr << "NTBP_Spampanato_WT_potassium_multi_current_o::StepCurrent()" << endl;
	NTreal tmpN = 0;
	NTsize counter = 0;
	if (!channelsPtr->getRatesComputed()) {
		ComputeRateConstants();
	}

	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION: {
		return (channelsPtr->BinomialStep(voltage));
	}
		break;
	case NTBP_GILLESPIE: {
		cerr << "WARNING : NTBP_GILLESPIE is being called on multi potassium channel. NOT IMPLEMENTED.";
		return NT_NOT_IMPLEMENTED;
	}
		break;
	case NTBP_SINGLECHANNEL: {
		return (channelsPtr->Step(voltage));
	}
		break;
	case NTBP_LANGEVIN: {
		/*counter = 0;
		m += _timeStep() * ((1.0 - m) * alphaM - m * betaM);
		NT_ASSERT(m>=0 && m<=1);
		do {
			counter++;
			tmpM = _timeStep() * normalRnd.RndVal() * sqrt((alphaM * (1 - m)
					+ betaM * m) / _numChannels());
			if (counter > 1 && counter < 1024)
				cerr << "NaM=" << counter << endl;
			else if (counter >= 1024) {
				noiseM = 0;
				tmpM = 0;
				break;
			}
		} while ((_timeStep() * (tmpM + noiseM) + m >= 1) || (_timeStep()
				* (tmpM + noiseM) + m <= 0));
		noiseM += tmpM;
		m += _timeStep() * noiseM;

		counter = 0;
		h += _timeStep() * ((1.0 - h) * alphaH - h * betaH);
		NT_ASSERT(h>=0 && h<= 1);
		do {
			counter++;
			tmpH = _timeStep() * normalRnd.RndVal() * sqrt((alphaH * (1 - h)
					+ betaH * h) / _numChannels());
			if (counter > 1 && counter < 1024)
				cerr << "NaH=" << counter << endl;
			else if (counter >= 1024) {
				noiseH = 0;
				tmpH = 0;
				break;
			}
		} while ((_timeStep() * (tmpH + noiseH) + h >= 1) || (_timeStep()
				* (tmpH + noiseH) + h <= 0));
		noiseH += tmpH;
		h += _timeStep() * noiseH;

		return NT_SUCCESS;*/
		cerr << "WARNING : NTBP_LANGEVIN is being called on multi potassium channel. NOT IMPLEMENTED.";
		return NT_NOT_IMPLEMENTED;
	}
		break;
	case NTBP_NOISYMEAN:
	case NTBP_DETERMINISTIC:
		n += _timeStep() * ((1.0 - n) * alphaN - n * betaN);
		NT_ASSERT(n>=0 && n<= 1);
		return NT_SUCCESS;
		break;
	default:
		cerr
				<< "NTBP_Spampanato_WT_potassium_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return NT_PARAM_UNSUPPORTED;
		break;
	}
	return NT_FAIL;
}

inline void NTBP_Spampanato_WT_potassium_multi_current_o::ComputeRateConstants() {
	cerr << "Calculating rate matrix for NTBP_Spampanato_WT_potassium_multi_current_o" << endl;
	NTreal temp = _temperature();
	NTreal deltaT = _timeStep();
	NTreal q10FactorN = NTBP_TemperatureRateRelation(temp, baseTemp /* C */,
			q10n);
	NTsize index = 0;
	NTreal vM = -100;

	for (NTsize i = 0; i < 5000; i++) {
		vM += 0.1;
		NTreal alphaN, betaN;
		if ((vM < -20) || (vM > 130.0)) {
			alphaN = q10FactorN * AlphaN(vM);
			betaN = q10FactorN * BetaN(vM);
		} else {
			index = (NTsize) floor((vM + 20) * 100.0);
			alphaN = q10FactorN * alphaNvec[index];
			betaN = q10FactorN * betaNvec[index];
		}

		NTreal alphaNdeltaT = alphaN * deltaT;
		NTreal betaNdeltaT = betaN * deltaT;

		channelsPtr->setTransactionProbability(i, 1, 2, 4 * alphaNdeltaT);
		channelsPtr->setTransactionProbability(i, 2, 3, 3 * alphaNdeltaT);
		channelsPtr->setTransactionProbability(i, 3, 4, 2 * alphaNdeltaT);
		channelsPtr->setTransactionProbability(i, 4, 5, 1 * alphaNdeltaT);

		channelsPtr->setTransactionProbability(i, 5, 4, 4 * betaNdeltaT);
		channelsPtr->setTransactionProbability(i, 4, 3, 3 * betaNdeltaT);
		channelsPtr->setTransactionProbability(i, 3, 2, 2 * betaNdeltaT);
		channelsPtr->setTransactionProbability(i, 2, 1, 1 * betaNdeltaT);


	}

	channelsPtr->setRatesComputed(true);
}

/**  */
/** No descriptions */
inline NTreal NTBP_Spampanato_WT_potassium_multi_current_o::OpenChannels() const {
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
		return channelsPtr->NumOpen();
		break;
	case NTBP_LANGEVIN:
	case NTBP_DETERMINISTIC:
		return pow(n,4) * NumChannels();
	default:
		cerr
				<< "NTBP_Spampanato_WT_potassium_multi_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels."
				<< endl;
		return 0;
	}
}

/**  */
/** No descriptions */
inline NTreal NTBP_Spampanato_WT_potassium_multi_current_o::OpenChannelsRatio() const {
	return OpenChannels() * 100 / NumChannels();
}

inline NTreal NTBP_Spampanato_WT_potassium_multi_current_o::ComputeConductance() {
	//  cerr << "NTBP_Spampanato_WT_potassium_multi_current_o::ComputeConductance" << endl;
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
		return Set_conductance(channelsPtr->NumOpen() * conductivity);
		break;
	case NTBP_LANGEVIN:
	case NTBP_DETERMINISTIC:
		return Set_conductance(_maxConductivity() /* mS/cm^2 */* pow(n,4) * _area()
		/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	case NTBP_NOISYMEAN: {
		/*NTreal mean = n^4;
		NTreal numAddOpening = m * m * (1 - m) * (1 - h) * alphaM / _timeStep()
				+ m * m * m * (1 - h) * alphaH / _timeStep();
		NTreal numAddClosing = m * m * m * h * (3 * betaM + betaH)
				/ _timeStep();
		mean += numAddOpening - numAddClosing;*/
		cerr << "NTBP_Spampanato_WT_potassium_multi_current_o::ComputeConductance(NTBP_NOISYMEAN): NOT IMPLEMENTED CORRECTLY" << endl;
		//return Set_conductance(_maxConductivity() /* mS/cm^2 */* mean * _area()
		return NT_NOT_IMPLEMENTED;
		// /* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	}
	default:
		cerr
				<< "NTBP_Spampanato_WT_potassium_multi_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance."
				<< endl;
		return 0;
	}
}

inline NTreal NTBP_Spampanato_WT_potassium_multi_current_o::ComputeChannelStateTimeConstant() const {
	// cerr << "NTBP_Spampanato_WT_potassium_multi_current_o::ComputeChannelStateTimeConstant()" << endl;
	return channelsPtr->ComputeChannelStateTimeConstant(voltage);
}

void NTBP_Spampanato_WT_potassium_multi_current_o::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}
