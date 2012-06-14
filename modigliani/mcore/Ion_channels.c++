/**
 * @file Ion_channels.c++
 * Ion_channels class implementation
 * @author Ahmed Aldo Faisal &copy; created 15.3.2001  
 * @version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * @section LICENSE
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

#include "Ion_channels.h"

using namespace mcore;

modigliani_base::Uniform_rnd_dist Ion_channels::uniformRnd;
modigliani_base::Binomial_rnd_dist Ion_channels::binomRnd(0.0, 1);

/* ***      CONSTRUCTORS	***/
Ion_channels::Ion_channels(modigliani_base::Size numNewChannels,
		modigliani_base::Size numNewStates, Transition_rate_matrix* probMatrix,
		modigliani_base::Real newTimeStep) :
		Object(), _probMatrix(probMatrix) {
	setTimeStep(newTimeStep);
	numChannels = numNewChannels;
	numStates = numNewStates;
	statePersistenceProbVec.resize(_numStates());
	stateCounterVec.resize(_numStates() + 1);
	uniformRnd = modigliani_base::Uniform_rnd_dist(0, 1);
	modigliani_base::Size tmpNumChannels = _numChannels();
	if (_numChannels() <= 1) {
		std::cerr << "Warning : _numChannels() <= 1, setting numChannels to 1."
				<< std::endl;
		tmpNumChannels = 1;
	}
	binomRnd = modigliani_base::Binomial_rnd_dist(0.0, tmpNumChannels);
	stateCounterVec[0] = _numChannels();
	stateCounterVec[1] = _numChannels();
	// Distribute channels evenly
	modigliani_base::Size delta = _numChannels() / numStates;
	for (modigliani_base::Size i = 1; i < numStates; i++) {
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
	stateCounterVec.resize(_numStates() + 1);
	uniformRnd = modigliani_base::Uniform_rnd_dist(0, 1);
	modigliani_base::Size tmpNumChannels = _numChannels();
	if (_numChannels() <= 1) {
		std::cerr << "Warning : _numChannels() <= 1, setting numChannels to 1."
				<< std::endl;
		tmpNumChannels = 1;
	}
	this->binomRnd = modigliani_base::Binomial_rnd_dist(0.0, tmpNumChannels);
	stateCounterVec[0] = original.numChannels;
	stateCounterVec[1] = original.numChannels;
	// Distribute channels evenly
	modigliani_base::Size delta = original.numChannels / original.numStates;
	for (modigliani_base::Size i = 1; i < numStates; i++) {
		stateCounterVec[1] -= delta;
		stateCounterVec[i + 1] = delta;
	}
	ratesComputed = false;
}

/** 
 * Copy constructor
 *
 * @param right
 *
 * @return
 */
Ion_channels&
Ion_channels::operator=(const Ion_channels & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/** 
 * Destructor
 * 
 */
Ion_channels::~Ion_channels() {
}

/* ***  PUBLIC                                    ***   */
/** 
 * Defines the given state as an open state
 * 
 * @param newOpenState 
 */
void Ion_channels::setAsOpenState(modigliani_base::Size newOpenState) {
	openStates.push_back(newOpenState);
}

/**  */
modigliani_base::ReturnEnum Ion_channels::GillespieStep(modigliani_base::Real voltage) {
	std::cerr << "NTBP_ion_channels_o::GillespieStep()" << std::endl;
	modigliani_base::Uniform_rnd_dist rnd;
	modigliani_base::Real val = rnd.RndVal();
	modigliani_base::Real deltaT = _timeStep();
	modigliani_base::Real channelTau = ComputeChannelStateTimeConstant(voltage);
	std::cerr << "channelTau=" << channelTau << std::endl;

	modigliani_base::Real sum = 0.0;
	for (modigliani_base::Size ll = 1; ll < _numStates() + 1; ll++) {
		modigliani_base::Real stateChangeProbability = 0;
		for (modigliani_base::Size nextState = 1; nextState < _numStates() + 1;
				nextState++) {
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
	return (modigliani_base::ReturnEnum::FAIL);
}

/**
 * Take a step using the single channel algorithm
 * @return
 */
modigliani_base::ReturnEnum Ion_channels::step(modigliani_base::Real voltage) {
	modigliani_base::Real rv = 0;
	std::vector<int> oldStateCounterVec = stateCounterVec;
	modigliani_base::Size matrix_index = _probMatrix->get_index(voltage);
	for (modigliani_base::Size current_state = 1; current_state < _numStates() + 1;
			current_state++) {
		for (int llc = 1; llc < oldStateCounterVec[current_state] + 1;
				llc++) {
			rv = uniformRnd.RndVal();
			modigliani_base::Real accumulatedProb = 0;
			for (modigliani_base::Size nextState = 1; nextState < _numStates() + 1;
					nextState++) {
				if (_probMatrix->getTransitionProbability(matrix_index,
						current_state, nextState) == 0)
					continue;
				if (nextState == current_state)
					continue;
				// This replaces awkward tNN probabilities
				accumulatedProb += _probMatrix->getTransitionProbability(
						matrix_index, current_state, nextState);
				if (rv <= accumulatedProb
						&& stateCounterVec[current_state] > 0) {
					// The state counter std::vector starts at 1
					stateCounterVec[nextState]++;
					stateCounterVec[current_state]--;

					// We have found our next state
					break;
				}
				std::cerr
						<< "Ion_channels::step : Something went wrong. The sum of all those probabilities should have reached one."
						<< std::endl;
				return (modigliani_base::ReturnEnum::FAIL);
			}

		}
	}
	return (modigliani_base::ReturnEnum::SUCCESS);
}

void Ion_channels::ShowStates() const {
	std::cout << "\tChannel:";
	for (modigliani_base::Size ll = 1; ll < _numStates() + 1; ll++)
		std::cout << stateCounterVec[ll] << " ";
	std::cout << std::endl;
}

/*
 *
 */
modigliani_base::Size Ion_channels::NumOpen() const {
	modigliani_base::Size count = 0;
	for (std::vector<modigliani_base::Size>::const_iterator it = openStates.begin();
			it != openStates.end(); ++it) {
		modigliani_base::Size state = *it;
		count += stateCounterVec[state];
	}
	return (count);
}

/** Sum of escape rates [1/s] */
modigliani_base::Real Ion_channels::ComputeChannelStateTimeConstant(
		modigliani_base::Real voltage) const {
	std::cerr << "NTBP_ion_channels_o::ComputeChannelStateTimeConstant()"
			<< std::endl;
	modigliani_base::Real sum = 0.0;
	modigliani_base::Real deltaT = _timeStep();
	for (modigliani_base::Size ll = 1; ll < _numStates() + 1; ll++) {
		modigliani_base::Real stateChangeProbability = 0;
		for (modigliani_base::Size nextState = 1; nextState < _numStates() + 1;
				nextState++) {
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
modigliani_base::ReturnEnum Ion_channels::ComputeGillespieStep(modigliani_base::Size stateId,
		modigliani_base::Real voltage) {
	std::cerr << "NTBP_ion_channels_o::ComputeGillespieStep" << std::endl;
	modigliani_base::Uniform_rnd_dist rnd;
	//int index = (voltage + 100) * 1000;
	// This operation is costly. So we do it only once.
	modigliani_base::Size matrix_index = _probMatrix->get_index(voltage);

	modigliani_base::Size oldOpen = NumOpen();
	modigliani_base::Real deltaT = _timeStep();

	modigliani_base::Real val = rnd.RndVal() * _timeStep();
	// the probability has to be converted into a rate

	modigliani_base::Real stateChangeProbability = 0;
	for (modigliani_base::Size nextState = 1; nextState < _numStates() + 1;
			nextState++) {
		if (stateId == nextState)
			continue;
		stateChangeProbability += _probMatrix->getTransitionProbability(
				matrix_index, stateId, nextState);
	}
	modigliani_base::Real accumulatedProb = 0;
	stateChangeProbability = stateChangeProbability / deltaT;
	for (modigliani_base::Size nextState = 1; nextState < _numStates() + 1;
			nextState++) {
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
	int check = 0.0;
	for (modigliani_base::Size ll = 1; ll < _numStates() + 1; ll++) {
		check += stateCounterVec[ll];
		std::cerr << check << std::endl;
	}
	std::cerr << _numChannels() << std::endl;
	M_ASSERT((unsigned int) check == _numChannels());

	if (NumOpen() == oldOpen)
		return (modigliani_base::ReturnEnum::FAIL);
	else
		return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Ion_channels::BinomialStep(modigliani_base::Real voltage) {
	std::vector<int> newStateCounterVec = stateCounterVec;
	// This operation is costly. So we do it only once.
	modigliani_base::Size matrix_index = _probMatrix->get_index(voltage);
	bool loop = false;
	modigliani_base::Size loopCounter = 0;

	do {
		loop = false;
		loopCounter++;
		for (modigliani_base::Size currentState = 1; currentState < _numStates() + 1;
				currentState++) {
			for (modigliani_base::Size nextState = 1; nextState < _numStates() + 1;
					nextState++) {
				if (nextState == currentState)
					continue;
				modigliani_base::Real prob = _probMatrix->getTransitionProbability(
						matrix_index, currentState, nextState);
				if (prob == 0)
					continue;
				//M_ASSERT(prob>0 && prob<=1);
				int numberOfChannels = stateCounterVec[currentState];
				modigliani_base::Real delta = binomRnd.Binomial(prob, numberOfChannels);
				newStateCounterVec[nextState] += delta;
				newStateCounterVec[currentState] -= delta;
			}
		}

		/* CHECKING CODE */
		modigliani_base::Size check = 0;
		for (modigliani_base::Size ll = 1; ll < _numStates() + 1; ll++) {
			if (newStateCounterVec[ll] < 0)
				loop = true;
			check += newStateCounterVec[ll];
		}
		if (check != _numChannels())
			loop = true;
	} while ((true == loop) && (loopCounter < 100));
	if (loopCounter >= 100) {
		std::cerr << "ERROR: Binominal step loop counter limit reached."
				<< std::endl;
		return (modigliani_base::ReturnEnum::SUCCESS);
	}
	stateCounterVec = newStateCounterVec;
	return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Ion_channels::DeterministicStep(modigliani_base::Real voltage) {
	std::vector<int> newStateCounterVec = stateCounterVec;
	// This operation is costly. So we do it only once.
	modigliani_base::Size matrix_index = _probMatrix->get_index(voltage);
	bool loop = false;
	modigliani_base::Size loopCounter = 0;

	do {
		loop = false;
		loopCounter++;
		for (modigliani_base::Size currentState = 1; currentState < _numStates() + 1;
				currentState++) {
			for (modigliani_base::Size nextState = 1; nextState < _numStates() + 1;
					nextState++) {
				if (nextState == currentState)
					continue;
				modigliani_base::Real prob = _probMatrix->getTransitionProbability(
						matrix_index, currentState, nextState);
				if (prob == 0)
					continue;
				//M_ASSERT(prob>0 && prob<=1);
				int numberOfChannels = stateCounterVec[currentState];
				int delta = floor(prob * numberOfChannels);
				newStateCounterVec[nextState] += delta;
				newStateCounterVec[currentState] -= delta;
			}
		}

		/* CHECKING CODE */
		modigliani_base::Size check = 0;
		for (modigliani_base::Size ll = 1; ll < _numStates() + 1; ll++) {
			if (newStateCounterVec[ll] < 0)
				loop = true;
			check += newStateCounterVec[ll];
		}
		if (check != _numChannels())
			loop = true;
	} while ((true == loop) && (loopCounter < 100));
	if (loopCounter >= 100) {
		std::cerr << "ERROR: Binominal step loop counter limit reached."
				<< std::endl;
		return (modigliani_base::ReturnEnum::SUCCESS);
	}
	stateCounterVec = newStateCounterVec;
	return (modigliani_base::ReturnEnum::SUCCESS);
}

/** Do not call before rate constants were set. */
modigliani_base::ReturnEnum Ion_channels::SteadyStateDistribution(modigliani_base::Real voltage) {
	// TODO dirty hack ... make time step dependent and maybe compute directly state
	//values
	for (modigliani_base::Size ll = 0; ll < 1000; ll++) {
		BinomialStep(voltage); //much faster then having to wait for Step()
	}
	return (modigliani_base::ReturnEnum::SUCCESS);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
