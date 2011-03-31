#include "ntbp_Spampanato_WT_sodium_multi_current_obj.h"

NT_gaussian_rnd_dist_o NTBP_Spampanato_WT_sodium_multi_current_o::normalRnd;
bool NTBP_Spampanato_WT_sodium_multi_current_o::initTableLookUp = false;
NTreal NTBP_Spampanato_WT_sodium_multi_current_o::alphaMvec[15000];
NTreal NTBP_Spampanato_WT_sodium_multi_current_o::betaMvec[15000];
NTreal NTBP_Spampanato_WT_sodium_multi_current_o::alphaHvec[15000];
NTreal NTBP_Spampanato_WT_sodium_multi_current_o::betaHvec[15000];
NTBP_transition_rate_matrix_o* NTBP_Spampanato_WT_sodium_multi_current_o::probMatrix;

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_Spampanato_WT_sodium_multi_current_o */
NTBP_Spampanato_WT_sodium_multi_current_o::NTBP_Spampanato_WT_sodium_multi_current_o(
		NTreal newArea, NTreal newDensity, NTreal newConductivity,
		NTreal newVBase, NTreal newQ10m, NTreal newQ10h,
		NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature) :
	q10h(newQ10h), q10m(newQ10m), NTBP_multi_current_o(
			reversalPotential /* in mV */, newDensity /* channels per mu^2 */,
			newArea /* in mu^2 */, newConductivity /* in mS per channel  */,
			newVBase /* mV */
	) {
	//density and area updated by NTBP_multi_current_obj constructor
	UpdateNumChannels();

	//TODO: What are noiseM / -H?
	noiseM = 0;
	noiseH = 0;
	baseTemp = 20.0; //all measurements undertaken at 20C
	setTimeStep(newTimeStep);
	Set_temperature(newTemperature);
	NTreal vTmp = 0;

	if (false == initTableLookUp) {
		for (NTsize ll = 0; ll < 15000; ll++) {
			vTmp = ((float) ll) / 100.0 - 20 + 0.005 /* otherwise we hit undefined points of alpha and beta function */;
			alphaMvec[ll] = AlphaM(vTmp);
			betaMvec[ll] = BetaM(vTmp);
			alphaHvec[ll] = AlphaH(vTmp);
			betaHvec[ll] = BetaH(vTmp);
		}
		probMatrix = new NTBP_transition_rate_matrix_o(8, -100, 300, 0.01);
		ComputeRateConstants();
		initTableLookUp = true;
	}
	channelsPtr = new NTBP_ion_channels_o(_numChannels(), 8, probMatrix,
			newTimeStep);
	channelsPtr->setAsOpenState(4);

	m = AlphaM(0) / (AlphaM(0) + BetaM(0));
	h = AlphaH(0) / (AlphaH(0) + BetaH(0));
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_Spampanato_WT_sodium_multi_current_o::NTBP_Spampanato_WT_sodium_multi_current_o(
		const NTBP_Spampanato_WT_sodium_multi_current_o & original) :
	q10h(original.q10h), q10m(original.q10m), NTBP_multi_current_o(
			original._reversalPotential(), original._density(),
			original._area(), original._conductivity()) {
	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 8,
			probMatrix, original._timeStep());
	channelsPtr->setAsOpenState(4);
}

const NTBP_Spampanato_WT_sodium_multi_current_o&
NTBP_Spampanato_WT_sodium_multi_current_o::operator=(
		const NTBP_Spampanato_WT_sodium_multi_current_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 8, probMatrix,
			right._timeStep());
	channelsPtr->setAsOpenState(4);
	return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_Spampanato_WT_sodium_multi_current_o::~NTBP_Spampanato_WT_sodium_multi_current_o() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline NTreturn NTBP_Spampanato_WT_sodium_multi_current_o::StepCurrent() {
	//	cerr << "NTBP_Spampanato_WT_sodium_multi_current_o::StepCurrent()" << endl;
	NTreal tmpM = 0;
	NTreal tmpH = 0;
	NTsize counter = 0;
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION: {
		return (channelsPtr->BinomialStep(voltage));
	}
		break;
	case NTBP_GILLESPIE: {
		cerr
				<< "WARNING : NTBP_GILLESPIE is being called on Spampanato WT sodium channel. NOT IMPLEMENTED.";
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
		cerr
				<< "WARNING : NTBP_LANGEVIN is being called on Spampanato WT sodium channel. NOT IMPLEMENTED.";
		return NT_NOT_IMPLEMENTED;
	}
		break;
	case NTBP_NOISYMEAN:
	case NTBP_DETERMINISTIC:
		m += _timeStep() * ((1.0 - m) * AlphaM(voltage) - m * BetaM(voltage));
		NT_ASSERT(m>=0 && m<= 1);
		h += _timeStep() * ((1.0 - h) * AlphaH(voltage) - h * BetaH(voltage));
		NT_ASSERT(h>=0 && h<= 1);
		return NT_SUCCESS;
		break;
	default:
		cerr
				<< "NTBP_Spampanato_WT_sodium_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return NT_PARAM_UNSUPPORTED;
		break;
	}
	return NT_FAIL;
}

inline void NTBP_Spampanato_WT_sodium_multi_current_o::ComputeRateConstants() {
	cerr
			<< "Calculating rate matrix for NTBP_Spampanato_WT_sodium_multi_current_o"
			<< endl;

	NTreal temp = _temperature();
	NTreal deltaT = _timeStep();
	NTreal q10FactorM = NTBP_TemperatureRateRelation(temp, baseTemp /* C */,
			q10m);
	NTreal q10FactorH = NTBP_TemperatureRateRelation(temp, baseTemp /* C */,
			q10h);
	NTsize index = 0;

	for (NTreal vM = -100; vM<300; vM+=0.01) {
		NTreal alphaM, betaM, alphaH, betaH;
		if ((vM < -20) || (vM > 130.0)) {
			alphaM = q10FactorM * AlphaM(vM);
			betaM = q10FactorM * BetaM(vM);

			alphaH = q10FactorH * AlphaH(vM);
			betaH = q10FactorH * BetaH(vM);
		} else {
			index = (NTsize) floor((vM + 20) * 100.0);
			alphaM = q10FactorM * alphaMvec[index];
			betaM = q10FactorM * betaMvec[index];

			alphaH = q10FactorH * alphaHvec[index];
			betaH = q10FactorH * betaHvec[index];
		}

		NTreal alphaMdeltaT = alphaM * deltaT;
		NTreal betaMdeltaT = betaM * deltaT;
		NTreal alphaHdeltaT = alphaH * deltaT;
		NTreal betaHdeltaT = betaH * deltaT;

		probMatrix->setTransitionProbability(vM, 1, 2, 3 * alphaMdeltaT);
		probMatrix->setTransitionProbability(vM, 2, 3, 2 * alphaMdeltaT);
		probMatrix->setTransitionProbability(vM, 3, 4, alphaMdeltaT);
		probMatrix->setTransitionProbability(vM, 5, 6,
				probMatrix->getTransitionProbability(vM, 1, 2));
		probMatrix->setTransitionProbability(vM, 6, 7,
				probMatrix->getTransitionProbability(vM, 2, 3));
		probMatrix->setTransitionProbability(vM, 7, 8,
				probMatrix->getTransitionProbability(vM, 3, 4));

		probMatrix->setTransitionProbability(vM, 8, 7, 3 * betaMdeltaT);
		probMatrix->setTransitionProbability(vM, 7, 6, 2 * betaMdeltaT);
		probMatrix->setTransitionProbability(vM, 6, 5, betaMdeltaT);
		probMatrix->setTransitionProbability(vM, 4, 3, 3 * betaMdeltaT);
		probMatrix->setTransitionProbability(vM, 3, 2,
				probMatrix->getTransitionProbability(vM, 7, 6));
		probMatrix->setTransitionProbability(vM, 2, 1,
				probMatrix->getTransitionProbability(vM, 6, 5));

		probMatrix->setTransitionProbability(vM, 1, 5, betaHdeltaT);
		probMatrix->setTransitionProbability(vM, 2, 6, betaHdeltaT);
		probMatrix->setTransitionProbability(vM, 3, 7, betaHdeltaT);
		probMatrix->setTransitionProbability(vM, 4, 8, betaHdeltaT);

		probMatrix->setTransitionProbability(vM, 5, 1, alphaHdeltaT);
		probMatrix->setTransitionProbability(vM, 6, 2, alphaHdeltaT);
		probMatrix->setTransitionProbability(vM, 7, 3, alphaHdeltaT);
		probMatrix->setTransitionProbability(vM, 8, 4, alphaHdeltaT);
	}
}

/**  */
/** No descriptions */
inline NTreal NTBP_Spampanato_WT_sodium_multi_current_o::OpenChannels() const {
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
		return channelsPtr->NumOpen();
		break;
	case NTBP_LANGEVIN:
	case NTBP_DETERMINISTIC:
		return m * m * m * h * NumChannels();
	default:
		cerr
				<< "NTBP_Spampanato_WT_sodium_multi_current_o::OpenChannels - ERROR : Unsupported simulation mode for OpenChannels."
				<< endl;
		return 0;
	}
}

/**  */
/** No descriptions */
inline NTreal NTBP_Spampanato_WT_sodium_multi_current_o::OpenChannelsRatio() const {
	return OpenChannels() * 100 / NumChannels();
}

inline NTreal NTBP_Spampanato_WT_sodium_multi_current_o::ComputeConductance() {
	//  cerr << "NTBP_Spampanato_WT_sodium_multi_current_o::ComputeConductance" << endl;
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION:
	case NTBP_GILLESPIE:
	case NTBP_SINGLECHANNEL:
	case NTBP_DETERMINISTIC:

		return Set_conductance(channelsPtr->NumOpen() * conductivity);
		break;
	case NTBP_LANGEVIN:
		return Set_conductance(_maxConductivity() /* mS/cm^2 */* m * m * m * h
				* _area()
		/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	case NTBP_NOISYMEAN: {
		NTreal mean = m * m * m * h;
		NTreal numAddOpening = m * m * (1 - m) * (1 - h) * alphaM / _timeStep()
				+ m * m * m * (1 - h) * alphaH / _timeStep();
		NTreal numAddClosing = m * m * m * h * (3 * betaM + betaH)
				/ _timeStep();
		mean += numAddOpening - numAddClosing;
		cerr << "NOT IMPLEMENTED CORRECTLY" << endl;
		return Set_conductance(_maxConductivity() /* mS/cm^2 */* mean * _area()
		/* muMeter^2 */* 1.0e-8 /* cm^2/muMeter^2 */);
		break;
	}
	default:
		cerr
				<< "NTBP_Spampanato_WT_sodium_multi_current_o::ComputeConductance - ERROR : Unsupported simulation mode for ComputeConductance."
				<< endl;
		return 0;
	}
}

inline NTreal NTBP_Spampanato_WT_sodium_multi_current_o::ComputeChannelStateTimeConstant() const {
	// cerr << "NTBP_Spampanato_WT_sodium_multi_current_o::ComputeChannelStateTimeConstant()" << endl;
	return channelsPtr->ComputeChannelStateTimeConstant(voltage);
}

void NTBP_Spampanato_WT_sodium_multi_current_o::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}
