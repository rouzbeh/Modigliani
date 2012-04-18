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
		NTsize numNewStates, NTBP_transition_rate_matrix_o* probMatrix,
		NTreal newTimeStep) :
		NTBP_object_o(), _probMatrix(probMatrix) {
	setTimeStep(newTimeStep);
	numChannels = numNewChannels;
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
	stateCounterVec[0] = _numChannels();
	stateCounterVec[1] = _numChannels();
	// Distribute channels evenly
	NTsize delta = _numChannels() / numStates;
	for (NTsize i = 1; i < numStates; i++) {
		stateCounterVec[1] -= delta;
		stateCounterVec[i + 1] = delta;
	}
	ratesComputed = false;
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_ion_channels_o::NTBP_ion_channels_o(const NTBP_ion_channels_o & original) :
		NTBP_object_o(), _probMatrix(original._probMatrix) {
	setTimeStep(original._timeStep());
	numChannels = original.numChannels;
	numStates = original.numStates;
	statePersistenceProbVec.resize(numStates);
	stateCounterVec.resize(_numStates() + 1); // 0: total number of channels, 1..8 state"besetzung"
	uniformRnd = NT_uniform_rnd_dist_o(0, 1);
	NTsize tmpNumChannels = _numChannels();
	if (_numChannels() <= 1) {
		cerr
				<< "NTBP_ion_channels_o::NTBP_ion_channels_o - Warning : _numChannels() <= 1, setting numChannels to 1 in binomial population algorithm."
				<< endl;
		tmpNumChannels = 1;
	}
	this->binomRnd = NT_binomial_rnd_dist_o(0.0, tmpNumChannels);
	stateCounterVec[0] = original.numChannels;
	stateCounterVec[1] = original.numChannels;
	// Distribute channels evenly
	NTsize delta = original.numChannels / original.numStates;
	for (NTsize i = 1; i < numStates; i++) {
		stateCounterVec[1] -= delta;
		stateCounterVec[i + 1] = delta;
	}
	ratesComputed = false;
}

const NTBP_ion_channels_o&
NTBP_ion_channels_o::operator=(const NTBP_ion_channels_o & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
NTBP_ion_channels_o::~NTBP_ion_channels_o() {
}

/* ***  PUBLIC                                    ***   */
void NTBP_ion_channels_o::setAsOpenState(NTsize newOpenState) {
	openStates.push_back(newOpenState);
}

/**  */
NTreturn NTBP_ion_channels_o::GillespieStep(NTreal voltage) {
	cerr << "NTBP_ion_channels_o::GillespieStep()" << endl;
	NT_uniform_rnd_dist_o rnd;
	NTreal val = rnd.RndVal();
	NTreal deltaT = _timeStep();
	NTreal channelTau = ComputeChannelStateTimeConstant(voltage);
	cerr << "channelTau=" << channelTau << endl;

	NTreal sum = 0.0;
	for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
		NTreal stateChangeProbability = 0;
		for (NTsize nextState = 1; nextState < _numStates() + 1; nextState++) {
			if (ll == nextState)
				continue;
			stateChangeProbability += _probMatrix->getTransitionProbability(
					voltage, ll, nextState);
		}
		sum += (stateChangeProbability / deltaT) * stateCounterVec[ll];
		cerr << "p=" << sum / (channelTau) << " val=" << val << endl;
		if (val < sum / channelTau) {
			return (ComputeGillespieStep(ll, voltage));
		}
	}
	cerr
			<< "NTBP_ion_channels_o::GillespieStep - Error : Control flow should not have reach here. No channel transition done."
			<< endl;
	return (NT_FAIL);
}

/**
 *
 * @return
 */
NTreturn NTBP_ion_channels_o::step(NTreal voltage) {
	NTreal rv = 0;
	vector<NTint> oldStateCounterVec = stateCounterVec;
	NTsize matrix_index = _probMatrix->get_index(voltage);
	for (NTsize lls = 1; lls < _numStates() + 1; lls++) {
		for (NTint llc = 1; llc < oldStateCounterVec[lls] + 1; llc++) {
			rv = uniformRnd.RndVal();
			NTreal accumulatedProb = 0;
			for (NTsize nextState = 0; nextState < _numStates(); nextState++) {
				if (_probMatrix->getTransitionProbability(matrix_index, lls,
						nextState) == 0)
					continue;

				// This replaces awkward tNN probabilities
				accumulatedProb += _probMatrix->getTransitionProbability(
						matrix_index, lls, nextState);
				if (rv <= accumulatedProb && stateCounterVec[lls] > 0) {
					// The state counter vector starts at 1
					stateCounterVec[nextState]++;
					stateCounterVec[lls]--;

					// We have found our next state
					break;
				}
				// Something went wrong. The sum of all those probabilities
				// should have reached one.
				return (NT_FAIL);
			}

		}
	}
	return (NT_SUCCESS);
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
	for (vector<NTsize>::const_iterator it = openStates.begin();
			it != openStates.end(); ++it) {
		NTsize state = *it;
		count += stateCounterVec[state];
	}
	return (count);
}

/** Sum of escape rates [1/s] */
NTreal NTBP_ion_channels_o::ComputeChannelStateTimeConstant(
		NTreal voltage) const {
	cerr << "NTBP_ion_channels_o::ComputeChannelStateTimeConstant()" << endl;
	NTreal sum = 0.0;
	NTreal deltaT = _timeStep();
	for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
		NTreal stateChangeProbability = 0;
		for (NTsize nextState = 1; nextState < _numStates() + 1; nextState++) {
			if (ll == nextState)
				continue;
			stateChangeProbability += _probMatrix->getTransitionProbability(
					voltage, ll, nextState);
		}
		sum += (stateChangeProbability / deltaT) * stateCounterVec[ll];
	}
	return (sum);
}

/**
 *
 * @param stateId
 * @return
 */
NTreturn NTBP_ion_channels_o::ComputeGillespieStep(NTsize stateId,
		NTreal voltage) {
	cerr << "NTBP_ion_channels_o::ComputeGillespieStep" << endl;
	NT_uniform_rnd_dist_o rnd;
	//int index = (voltage + 100) * 1000;
	// This operation is costly. So we do it only once.
	NTsize matrix_index = _probMatrix->get_index(voltage);

	NTsize oldOpen = NumOpen();
	NTreal deltaT = _timeStep();

	NTreal val = rnd.RndVal() * _timeStep();
	// the probability has to be converted into a rate

	NTreal stateChangeProbability = 0;
	for (NTsize nextState = 1; nextState < _numStates() + 1; nextState++) {
		if (stateId == nextState)
			continue;
		stateChangeProbability += _probMatrix->getTransitionProbability(
				matrix_index, stateId, nextState);
	}
	NTreal accumulatedProb = 0;
	stateChangeProbability = stateChangeProbability / deltaT;
	for (NTsize nextState = 1; nextState < _numStates() + 1; nextState++) {
		if (stateId == nextState
				|| !_probMatrix->getTransitionProbability(matrix_index, stateId,
						nextState))
			continue;

		accumulatedProb += _probMatrix->getTransitionProbability(matrix_index,
				stateId, nextState) / stateChangeProbability;

		// We want to ensure change in the state even if float division induces errors
		if (val <= accumulatedProb || nextState == _numStates() - 1) {
			// The state counter vector starts at 1
			stateCounterVec[nextState]++;
			stateCounterVec[stateId]--;
			break;
		}
	}

	/* CHECKING CODE */
	NTint check = 0.0;
	for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
		check += stateCounterVec[ll];
		cerr << check << endl;
	}
	cerr << _numChannels() << endl;
	NT_ASSERT((unsigned int) check == _numChannels());

	if (NumOpen() == oldOpen)
		return (NT_FAIL);
	else
		return (NT_SUCCESS);
}

NTreturn NTBP_ion_channels_o::BinomialStep(NTreal voltage) {
	vector<NTint> newStateCounterVec = stateCounterVec;
	// This operation is costly. So we do it only once.
	NTsize matrix_index = _probMatrix->get_index(voltage);
	bool loop = false;
	NTsize loopCounter = 0;

	do {
		loop = false;
		loopCounter++;
		for (NTsize currentState = 1; currentState < _numStates() + 1;
				currentState++) {
			for (NTsize nextState = 1; nextState < _numStates() + 1;
					nextState++) {
				if (nextState == currentState)
					continue;
				NTreal prob = _probMatrix->getTransitionProbability(
						matrix_index, currentState, nextState);
				if (prob == 0)
					continue;
				//NT_ASSERT(prob>0 && prob<=1);
				int numberOfChannels = stateCounterVec[currentState];
				NTreal delta = binomRnd.Binomial(prob, numberOfChannels);
				newStateCounterVec[nextState] += delta;
				newStateCounterVec[currentState] -= delta;
			}
		}

		/* CHECKING CODE */
		NTsize check = 0;
		for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
			if (newStateCounterVec[ll] < 0)
				loop = true;
			check += newStateCounterVec[ll];
		}
		if (check != _numChannels())
			loop = true;
		} while ((true == loop) && (loopCounter < 100));
	if (loopCounter >= 100) {
		cerr << "ERROR: Binominal step loop counter limit reached." << endl;
		return (NT_SUCCESS);
	}
	stateCounterVec = newStateCounterVec;
	//	else {}
	//TODO: added: all state counters shown in BinominalStep
	//	for (NTsize ll = 1; ll < _numStates() + 1; ll++)
	//	{
	//		cerr << ll << ": " << stateCounterVec[ll] << "\t";
	//	}
	//	cerr << "\n" << endl;
	return (NT_SUCCESS);
}

NTreturn NTBP_ion_channels_o::DeterministicStep(NTreal voltage) {
	vector<NTint> newStateCounterVec = stateCounterVec;
	NTsize matrix_index = _probMatrix->get_index(voltage);

	for (NTsize currentState = 1; currentState < _numStates() + 1;
			currentState++) {
		for (NTsize nextState = 1; nextState < _numStates() + 1; nextState++) {
			if (nextState == currentState)
				continue;
			NTreal prob = _probMatrix->getTransitionProbability(matrix_index,
					currentState, nextState);
			if (prob == 0)
				continue;
			NTint numberOfChannels = stateCounterVec[currentState];
			NTint delta = floor(prob * numberOfChannels);
			newStateCounterVec[nextState] += delta;
			newStateCounterVec[currentState] -= delta;
		}
	}
	NTsize check = 0;
	bool loop = false;
	for (NTsize ll = 1; ll < _numStates() + 1; ll++) {
		if (newStateCounterVec[ll] < 0)
			loop = true;
		check += newStateCounterVec[ll];
	}
	if (check != _numChannels())
		loop = true;
	if (loop) {
		for (NTsize currentState = 1; currentState < _numStates() + 1;
				currentState++) {
			for (NTsize nextState = 1; nextState < _numStates() + 1;
					nextState++) {
				if (nextState == currentState)
					continue;
				NTreal prob = _probMatrix->getTransitionProbability(
						matrix_index, currentState, nextState);
				if (prob == 0)
					continue;
				NTint numberOfChannels = stateCounterVec[currentState];
				NTint delta = floor(prob * numberOfChannels);
				if (delta <= newStateCounterVec[currentState]) {
					newStateCounterVec[nextState] += delta;
					newStateCounterVec[currentState] -= delta;
				} else {
					newStateCounterVec[nextState] +=
							newStateCounterVec[currentState];
					newStateCounterVec[currentState] = 0;
				}
			}
		}
	}
	stateCounterVec = newStateCounterVec;
	return (NT_SUCCESS);
}

/** Do not call before rate constants were set. */
NTreturn NTBP_ion_channels_o::SteadyStateDistribution(NTreal voltage) {
	// TODO dirty hack ... make time step dependent and maybe compute directly state
	//values
	for (NTsize ll = 0; ll < 1000; ll++) {
		BinomialStep(voltage); //much faster then having to wait for Step()
	}
	return (NT_SUCCESS);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
