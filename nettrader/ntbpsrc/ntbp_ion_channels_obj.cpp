/**\file ntbp_ion_channels_obj.cpp - NTBP_ion_channels_o class implementation 
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

/* $Id: ntbp_ion_channels_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_ion_channels_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 */
#include "ntbp_ion_channels_obj.h"

NT_uniform_rnd_dist_o NTBP_ion_channels_o::uniformRnd;
NT_binomial_rnd_dist_o NTBP_ion_channels_o::binomRnd(0.0, 1);

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_ion_channels_o */
NTBP_ion_channels_o::NTBP_ion_channels_o(NTsize numNewChannels,
		NTsize numNewStates, NTreal newTimeStep) :
	NTBP_object_o(),
			_probMatrices(boost::extents[5000][numNewStates][numNewStates]) {
	NT_ASSERT(numNewChannels >= 0);
	setTimeStep(newTimeStep);
	numChannels = numNewChannels;
	NT_ASSERT(numNewStates >= 0);
	numStates = numNewStates;
	statePersistenceProbVec.resize(_numStates());
	stateCounterVec.resize(_numStates() + 1); // 0: total number of channels, 1..8 state"besetzung"
	uniformRnd = NT_uniform_rnd_dist_o(0, 1);
	NTsize tmpNumChannels = _numChannels();
	if (_numChannels() <= 1) {
		cerr
				<< "NTBP_ion_channels_o::NTBP_ion_channels_o - Warning : _numChannels() <= 1, setting numChannels to 1 in binomial population algorithm."
				<< endl;
		tmpNumChannels = 1;
	}
	binomRnd = NT_binomial_rnd_dist_o(0.0, tmpNumChannels);
	for (NTsize ll = 0; ll < _numStates() + 1; ll++)
		stateCounterVec[ll] = 0;
	stateCounterVec[0] = _numChannels();
	stateCounterVec[1] = _numChannels();
	ratesComputed = false;
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_ion_channels_o::NTBP_ion_channels_o(const NTBP_ion_channels_o & original) {
	// add assignment code here
}

const NTBP_ion_channels_o&
NTBP_ion_channels_o::operator=(const NTBP_ion_channels_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_ion_channels_o::~NTBP_ion_channels_o() {
}

/* ***  PUBLIC                                    ***   */

void NTBP_ion_channels_o::setAsOpenState(NTsize newOpenState) {
	openStates.push_back(newOpenState);
}

/**  */
bool NTBP_ion_channels_o::GillespieStep(NTreal voltage) {
	cerr << "NTBP_ion_channels_o::GillespieStep()" << endl;
	int index = (voltage + 100) / 1000;
	NT_uniform_rnd_dist_o rnd;
	NTreal val = rnd.RndVal();
	NTreal deltaT = _timeStep();
	NTreal channelTau = ComputeChannelStateTimeConstant(voltage);
	cerr << "channelTau=" << channelTau << endl;

	NTreal sum = 0.0;
	for (NTsize ll = 0; ll < _numStates(); ll++) {
		NTreal stateChangeProbability = 0;
		for (NTsize nextState = 0; nextState < _numStates(); nextState++) {
			if (ll == nextState)
				continue;
			stateChangeProbability += _probMatrices[index][ll][nextState];
		}
		sum += (stateChangeProbability / deltaT) * stateCounterVec[ll + 1];
		cerr << "p=" << sum / (channelTau) << " val=" << val << endl;
		if (val < sum / channelTau) {
			return ComputeGillespieStep(ll + 1, voltage);
		}
	}
	cerr
			<< "NTBP_ion_channels_o::GillespieStep - Error : Control flow should not have reach here. No channel transition done."
			<< endl;
	return NT_FAIL;
}

/**
 *
 * @return
 */
NTreturn NTBP_ion_channels_o::Step(NTreal voltage) {
	NTreal rv = 0;
	vector<NTsize> oldStateCounterVec = stateCounterVec;
	int index = (voltage + 100) / 1000;
	for (NTsize lls = 0; lls < _numStates(); lls++) {
		for (NTsize llc = 0; llc < oldStateCounterVec[lls + 1]; llc++) {
			rv = uniformRnd.RndVal();
			NTreal accumulatedProb = 0;
			for (NTsize nextState = 0; nextState < _numStates(); nextState++) {
				if (_probMatrices[index][lls][nextState] == 0)
					continue;

				// This replaces awkward tNN probabilities
				accumulatedProb += _probMatrices[index][lls][nextState];
				if (rv <= accumulatedProb) {
					// The state counter vector starts at 1
					stateCounterVec[nextState + 1]++;
					stateCounterVec[lls + 1]--;

					// We have found our next state
					break;
				}
				// Something went wrong. The sum of all those probabilities
				// should have reached one.
				return NT_FAIL;
			}

		}
	}
	return NT_SUCCESS;
}

void NTBP_ion_channels_o::ShowStates() const {
	cout << "\tChannel:";
	for (NTsize ll = 1; ll < _numStates() + 1; ll++)
		cout << stateCounterVec[ll] << " ";
	cout << endl;
}

/*
 *
 */
NTsize NTBP_ion_channels_o::NumOpen() const {
	NTsize count = 0;
	for (vector<NTsize>::const_iterator it = openStates.begin(); it
			!= openStates.end(); ++it) {
		int test = *it;
		count += stateCounterVec[*it];
	}
	return count;
}

/** Sum of escape rates [1/s] */
NTreal NTBP_ion_channels_o::ComputeChannelStateTimeConstant(NTreal voltage) const {
	cerr << "NTBP_ion_channels_o::ComputeChannelStateTimeConstant()" << endl;
	int index = (voltage + 100) / 1000;
	NTreal sum = 0.0;
	NTreal deltaT = _timeStep();
	for (NTsize ll = 0; ll < _numStates(); ll++) {
		NTreal stateChangeProbability = 0;
		for (NTsize nextState = 0; nextState < _numStates(); nextState++) {
			if (ll == nextState)
				continue;
			stateChangeProbability += _probMatrices[index][ll][nextState];
		}
		sum += (stateChangeProbability / deltaT) * stateCounterVec[ll + 1];
	}
	return sum;
}

/**
 *
 * @param stateId
 * @return
 */
bool NTBP_ion_channels_o::ComputeGillespieStep(NTsize stateId, NTreal voltage) {
	cerr << "NTBP_ion_channels_o::ComputeGillespieStep" << endl;
	NT_uniform_rnd_dist_o rnd;
	int index = (voltage + 100) * 1000;

	NTsize oldOpen = NumOpen();
	NTreal deltaT = _timeStep();

	NTreal val = rnd.RndVal() * _timeStep();
	// the probability has to be converted into a rate
	for (NTsize lls = 0; lls < _numStates(); lls++) {
		NTreal stateChangeProbability = 0;
		for (NTsize nextState = 0; nextState < _numStates(); nextState++) {
			if (lls == nextState)
				continue;
			stateChangeProbability += _probMatrices[index][lls][nextState];
		}
		NTreal accumulatedProb = 0;
		stateChangeProbability = stateChangeProbability / deltaT;
		for (NTsize nextState = 0; nextState < _numStates(); nextState++) {
			if (lls == nextState || !_probMatrices[index][lls][nextState])
				continue;

			accumulatedProb += _probMatrices[index][lls][nextState]
					/ stateChangeProbability;

			// We want to ensure change in the state even if float division induces errors
			if (val <= accumulatedProb || nextState == _numStates() - 1) {
				// The state counter vector starts at 1
				stateCounterVec[nextState + 1]++;
				stateCounterVec[lls + 1]--;
				break;
			}
		}
	}

	//	ShowStates();
	/* CHECKING CODE */
	NTint check = 0.0;
	for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
		check += stateCounterVec[ll];
		cerr << check << endl;
	}
	cerr << _numChannels() << endl;
	NT_ASSERT(check == _numChannels());

	if (NumOpen() == oldOpen)
		return false;
	else
		return true;
}

void NTBP_ion_channels_o::setTransactionProbability(NTreal voltage,
		NTsize start, NTsize stop, NTreal probability) {
	int index = (voltage * 10) + 1000;
	_probMatrices[index][start-1][stop-1] = probability;
}

NTreal NTBP_ion_channels_o::getTransactionProbability(NTreal voltage,
		NTsize start, NTsize stop) {
	NTsize index = (voltage * 10) + 1000;
	return getTransactionProbability(index, start, stop);
}

void NTBP_ion_channels_o::setTransactionProbability(NTsize index, NTsize start,
		NTsize stop, NTreal probability) {
	_probMatrices[index][start-1][stop-1] = probability;
}

NTreal NTBP_ion_channels_o::getTransactionProbability(NTsize index,
		NTsize start, NTsize stop) {
	return _probMatrices[index][start-1][stop-1];
}

inline NTreturn NTBP_ion_channels_o::BinomialStep(NTreal voltage) {
	vector<NTsize> oldStateCounterVec = stateCounterVec;
	vector<NTint> newStateCounterVec(stateCounterVec.size());
	for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
		newStateCounterVec[ll] = stateCounterVec[ll];
	}

	bool loop = false;
	NTsize loopCounter = 0;

	do {
		loop = false;
		loopCounter++;

		for (NTsize currentState = 1; currentState < _numStates()+1; currentState++) {
			for (NTsize nextState = 1; nextState < _numStates()+1; nextState++) {
				if (nextState == currentState
						|| getTransactionProbability(voltage, currentState, nextState)==0)
					continue;
				NTreal prob = getTransactionProbability(voltage, currentState, nextState);
				NTsize numberOfChannels = oldStateCounterVec[currentState];
				NTreal delta = binomRnd.Binomial(
						prob,
						oldStateCounterVec[currentState]);
				newStateCounterVec[nextState] += delta;
				newStateCounterVec[currentState] -= delta;
			}
		}

		/* CHECKING CODE */
		NTsize check = 0;
		for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
			check += newStateCounterVec[ll];
			/** if this bails out here it means that the step sizes (i.e. probabilities) are too large */
			if (newStateCounterVec[ll] < 0)
				loop = true;
		}
		if (check != _numChannels())
			loop = true;

		// after 100 attempts to find an adequate distribution,
		// ignore any transitions and "skip" this step
	} while ((true == loop) && (loopCounter < 100));
	if (loopCounter < 100) {
		for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
			stateCounterVec[ll] = newStateCounterVec[ll];
		}
	}
	return NT_SUCCESS;
}

/** Do not call before rate constants were set. */
#
inline NTreturn NTBP_ion_channels_o::SteadyStateDistribution(NTreal voltage) {
	// TODO dirty hack ... make time step dependent and maybe compute directly state
	//values
	for (NTsize ll = 0; ll < 1000; ll++) {
		BinomialStep(voltage); //much faster then having to wait for Step()
	}
	return NT_SUCCESS;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
