/**\file Ion_channelsbj.cpp - Ion_channels class implementation
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
#include "Ion_channels.h"

using namespace mcore;

mbase::Uniform_rnd_dist Ion_channels::uniformRnd;
mbase::Binomial_rnd_dist Ion_channels::binomRnd(0.0, 1);

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_ion_channels_o */
Ion_channels::Ion_channels(mbase::Size_t numNewChannels,
		mbase::Size_t numNewStates, Transition_rate_matrix* probMatrix,
		mbase::Real newTimeStep) :
		Object(), _probMatrix(probMatrix) {
	setTimeStep(newTimeStep);
	numChannels = numNewChannels;
	numStates = numNewStates;
	statePersistenceProbVec.resize(_numStates());
	stateCounterVec.resize(_numStates() + 1); // 0: total number of channels, 1..8 state"besetzung"
	uniformRnd = mbase::Uniform_rnd_dist(0, 1);
	mbase::Size_t tmpNumChannels = _numChannels();
	if (_numChannels() <= 1) {
		std::cerr
				<< "NTBP_ion_channels_o::NTBP_ion_channels_o - Warning : _numChannels() <= 1, setting numChannels to 1 in binomial population algorithm."
				<< std::endl;
		tmpNumChannels = 1;
	}
	binomRnd = mbase::Binomial_rnd_dist(0.0, tmpNumChannels);
	stateCounterVec[0] = _numChannels();
	stateCounterVec[1] = _numChannels();
	// Distribute channels evenly
	mbase::Size_t delta = _numChannels() / numStates;
	for (mbase::Size_t i = 1; i < numStates; i++) {
		stateCounterVec[1] -= delta;
		stateCounterVec[i + 1] = delta;
	}
	ratesComputed = false;
}

/* ***      COPY AND ASSIGNMENT	***/
Ion_channels::Ion_channels(const Ion_channels & original) :
		Object(), _probMatrix(original._probMatrix) {
	setTimeStep(original._timeStep());
	numChannels = original.numChannels;
	numStates = original.numStates;
	statePersistenceProbVec.resize(numStates);
	stateCounterVec.resize(_numStates() + 1); // 0: total number of channels, 1..8 state"besetzung"
	uniformRnd = mbase::Uniform_rnd_dist(0, 1);
	mbase::Size_t tmpNumChannels = _numChannels();
	if (_numChannels() <= 1) {
		std::cerr
				<< "NTBP_ion_channels_o::NTBP_ion_channels_o - Warning : _numChannels() <= 1, setting numChannels to 1 in binomial population algorithm."
				<< std::endl;
		tmpNumChannels = 1;
	}
	this->binomRnd = mbase::Binomial_rnd_dist(0.0, tmpNumChannels);
	stateCounterVec[0] = original.numChannels;
	stateCounterVec[1] = original.numChannels;
	// Distribute channels evenly
	mbase::Size_t delta = original.numChannels / original.numStates;
	for (mbase::Size_t i = 1; i < numStates; i++) {
		stateCounterVec[1] -= delta;
		stateCounterVec[i + 1] = delta;
	}
	ratesComputed = false;
}

const Ion_channels&
Ion_channels::operator=(const Ion_channels & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Ion_channels::~Ion_channels() {
}

/* ***  PUBLIC                                    ***   */
void Ion_channels::setAsOpenState(mbase::Size_t newOpenState) {
	openStates.push_back(newOpenState);
}

/**  */
mbase::Mreturn Ion_channels::GillespieStep(mbase::Real voltage) {
	std::cerr << "NTBP_ion_channels_o::GillespieStep()" << std::endl;
	mbase::Uniform_rnd_dist rnd;
	mbase::Real val = rnd.RndVal();
	mbase::Real deltaT = _timeStep();
	mbase::Real channelTau = ComputeChannelStateTimeConstant(voltage);
	std::cerr << "channelTau=" << channelTau << std::endl;

	mbase::Real sum = 0.0;
	for (mbase::Size_t ll = 1; ll < _numStates() + 1; ll++) {
		mbase::Real stateChangeProbability = 0;
		for (mbase::Size_t nextState = 1; nextState < _numStates() + 1; nextState++) {
			if (ll == nextState)
				continue;
			stateChangeProbability += _probMatrix->getTransitionProbability(
					voltage, ll, nextState);
		}
		sum += (stateChangeProbability / deltaT) * stateCounterVec[ll];
		std::cerr << "p=" << sum / (channelTau) << " val=" << val << std::endl;
		if (val < sum / channelTau) {
			return (ComputeGillespieStep(ll, voltage));
		}
	}
	std::cerr
			<< "NTBP_ion_channels_o::GillespieStep - Error : Control flow should not have reach here. No channel transition done."
			<< std::endl;
	return (mbase::M_FAIL);
}

/**
 *
 * @return
 */
mbase::Mreturn Ion_channels::step(mbase::Real voltage) {
	mbase::Real rv = 0;
	std::vector<mbase::Mint> oldStateCounterVec = stateCounterVec;
	mbase::Size_t matrix_index = _probMatrix->get_index(voltage);
	for (mbase::Size_t lls = 1; lls < _numStates() + 1; lls++) {
		for (mbase::Mint llc = 1; llc < oldStateCounterVec[lls] + 1; llc++) {
			rv = uniformRnd.RndVal();
			mbase::Real accumulatedProb = 0;
			for (mbase::Size_t nextState = 0; nextState < _numStates(); nextState++) {
				if (_probMatrix->getTransitionProbability(matrix_index, lls,
						nextState) == 0)
					continue;

				// This replaces awkward tNN probabilities
				accumulatedProb += _probMatrix->getTransitionProbability(
						matrix_index, lls, nextState);
				if (rv <= accumulatedProb && stateCounterVec[lls] > 0) {
					// The state counter std::vector starts at 1
					stateCounterVec[nextState]++;
					stateCounterVec[lls]--;

					// We have found our next state
					break;
				}
				// Something went wrong. The sum of all those probabilities
				// should have reached one.
				return (mbase::M_FAIL);
			}

		}
	}
	return (mbase::M_SUCCESS);
}

void Ion_channels::ShowStates() const {
	std::cout << "\tChannel:";
	for (mbase::Size_t ll = 1; ll < _numStates() + 1; ll++)
		std::cout << stateCounterVec[ll] << " ";
	std::cout << std::endl;
}

/*
 *
 */
mbase::Size_t Ion_channels::NumOpen() const {
	mbase::Size_t count = 0;
	for (std::vector<mbase::Size_t>::const_iterator it = openStates.begin();
			it != openStates.end(); ++it) {
		mbase::Size_t state = *it;
		count += stateCounterVec[state];
	}
	return (count);
}

/** Sum of escape rates [1/s] */
mbase::Real Ion_channels::ComputeChannelStateTimeConstant(
		mbase::Real voltage) const {
	std::cerr << "NTBP_ion_channels_o::ComputeChannelStateTimeConstant()" << std::endl;
	mbase::Real sum = 0.0;
	mbase::Real deltaT = _timeStep();
	for (mbase::Size_t ll = 1; ll < _numStates() + 1; ll++) {
		mbase::Real stateChangeProbability = 0;
		for (mbase::Size_t nextState = 1; nextState < _numStates() + 1; nextState++) {
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
mbase::Mreturn Ion_channels::ComputeGillespieStep(mbase::Size_t stateId,
		mbase::Real voltage) {
	std::cerr << "NTBP_ion_channels_o::ComputeGillespieStep" << std::endl;
	mbase::Uniform_rnd_dist rnd;
	//int index = (voltage + 100) * 1000;
	// This operation is costly. So we do it only once.
	mbase::Size_t matrix_index = _probMatrix->get_index(voltage);

	mbase::Size_t oldOpen = NumOpen();
	mbase::Real deltaT = _timeStep();

	mbase::Real val = rnd.RndVal() * _timeStep();
	// the probability has to be converted into a rate

	mbase::Real stateChangeProbability = 0;
	for (mbase::Size_t nextState = 1; nextState < _numStates() + 1; nextState++) {
		if (stateId == nextState)
			continue;
		stateChangeProbability += _probMatrix->getTransitionProbability(
				matrix_index, stateId, nextState);
	}
	mbase::Real accumulatedProb = 0;
	stateChangeProbability = stateChangeProbability / deltaT;
	for (mbase::Size_t nextState = 1; nextState < _numStates() + 1; nextState++) {
		if (stateId == nextState
				|| !_probMatrix->getTransitionProbability(matrix_index, stateId,
						nextState))
			continue;

		accumulatedProb += _probMatrix->getTransitionProbability(matrix_index,
				stateId, nextState) / stateChangeProbability;

		// We want to ensure change in the state even if float division induces errors
		if (val <= accumulatedProb || nextState == _numStates() - 1) {
			// The state counter std::vector starts at 1
			stateCounterVec[nextState]++;
			stateCounterVec[stateId]--;
			break;
		}
	}

	/* CHECKING CODE */
	mbase::Mint check = 0.0;
	for (mbase::Size_t ll = 1; ll < _numStates() + 1; ll++) {
		check += stateCounterVec[ll];
		std::cerr << check << std::endl;
	}
	std::cerr << _numChannels() << std::endl;
	M_ASSERT((unsigned int) check == _numChannels());

	if (NumOpen() == oldOpen)
		return (mbase::M_FAIL);
	else
		return (mbase::M_SUCCESS);
}

mbase::Mreturn Ion_channels::BinomialStep(mbase::Real voltage) {
	std::vector<mbase::Mint> newStateCounterVec = stateCounterVec;
	// This operation is costly. So we do it only once.
	mbase::Size_t matrix_index = _probMatrix->get_index(voltage);
	bool loop = false;
	mbase::Size_t loopCounter = 0;

	do {
		loop = false;
		loopCounter++;
		for (mbase::Size_t currentState = 1; currentState < _numStates() + 1;
				currentState++) {
			for (mbase::Size_t nextState = 1; nextState < _numStates() + 1;
					nextState++) {
				if (nextState == currentState)
					continue;
				mbase::Real prob = _probMatrix->getTransitionProbability(
						matrix_index, currentState, nextState);
				if (prob == 0)
					continue;
				//M_ASSERT(prob>0 && prob<=1);
				int numberOfChannels = stateCounterVec[currentState];
				mbase::Real delta = binomRnd.Binomial(prob, numberOfChannels);
				newStateCounterVec[nextState] += delta;
				newStateCounterVec[currentState] -= delta;
			}
		}

		/* CHECKING CODE */
		mbase::Size_t check = 0;
		for (mbase::Size_t ll = 1; ll < _numStates() + 1; ll++) {
			if (newStateCounterVec[ll] < 0)
				loop = true;
			check += newStateCounterVec[ll];
		}
		if (check != _numChannels())
			loop = true;
		} while ((true == loop) && (loopCounter < 100));
	if (loopCounter >= 100) {
		std::cerr << "ERROR: Binominal step loop counter limit reached." << std::endl;
		return (mbase::M_SUCCESS);
	}
	stateCounterVec = newStateCounterVec;
	//	else {}
	//TODO: added: all state counters shown in BinominalStep
	//	for (mbase::Size_t ll = 1; ll < _numStates() + 1; ll++)
	//	{
	//		std::cerr << ll << ": " << stateCounterVec[ll] << "\t";
	//	}
	//	std::cerr << "\n" << std::endl;
	return (mbase::M_SUCCESS);
}

mbase::Mreturn Ion_channels::DeterministicStep(mbase::Real voltage) {
	std::vector<mbase::Mint> newStateCounterVec = stateCounterVec;
	mbase::Size_t matrix_index = _probMatrix->get_index(voltage);

	for (mbase::Size_t currentState = 1; currentState < _numStates() + 1;
			currentState++) {
		for (mbase::Size_t nextState = 1; nextState < _numStates() + 1; nextState++) {
			if (nextState == currentState)
				continue;
			mbase::Real prob = _probMatrix->getTransitionProbability(matrix_index,
					currentState, nextState);
			if (prob == 0)
				continue;
			mbase::Mint numberOfChannels = stateCounterVec[currentState];
			mbase::Mint delta = floor(prob * numberOfChannels);
			newStateCounterVec[nextState] += delta;
			newStateCounterVec[currentState] -= delta;
		}
	}
	mbase::Size_t check = 0;
	bool loop = false;
	for (mbase::Size_t ll = 1; ll < _numStates() + 1; ll++) {
		if (newStateCounterVec[ll] < 0)
			loop = true;
		check += newStateCounterVec[ll];
	}
	if (check != _numChannels())
		loop = true;
	if (loop) {
		for (mbase::Size_t currentState = 1; currentState < _numStates() + 1;
				currentState++) {
			for (mbase::Size_t nextState = 1; nextState < _numStates() + 1;
					nextState++) {
				if (nextState == currentState)
					continue;
				mbase::Real prob = _probMatrix->getTransitionProbability(
						matrix_index, currentState, nextState);
				if (prob == 0)
					continue;
				mbase::Mint numberOfChannels = stateCounterVec[currentState];
				mbase::Mint delta = floor(prob * numberOfChannels);
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
	return (mbase::M_SUCCESS);
}

/** Do not call before rate constants were set. */
mbase::Mreturn Ion_channels::SteadyStateDistribution(mbase::Real voltage) {
	// TODO dirty hack ... make time step dependent and maybe compute directly state
	//values
	for (mbase::Size_t ll = 0; ll < 1000; ll++) {
		BinomialStep(voltage); //much faster then having to wait for Step()
	}
	return (mbase::M_SUCCESS);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
