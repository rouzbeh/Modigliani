/**
 * @file ion_channels.cc
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

#include "modigliani_core/ion_channels.h"

namespace modigliani_core {

bool Ion_channels::seed_set_ = 0;

// Constructors
Ion_channels::Ion_channels(modigliani_base::Size numNewChannels,
                           modigliani_base::Size numNewStates,
                           Transition_rate_matrix* probMatrix,
                           modigliani_base::Real newTimeStep)
    : Object(), _probMatrix(probMatrix), num_channels_(numNewChannels), num_states_(
        numNewStates) {

  setTimeStep(newTimeStep);

  statePersistenceProbVec.resize(num_states());
  stateCounterVec.resize(num_states() + 1);
  
  if (!Ion_channels::seed_set_) {
    Ion_channels::seed = time(NULL);
    std::cout << "Initial seed = " << Ion_channels::seed << std::endl;
    Ion_channels::seed_set_ = true;
  }
  rng = boost::random::mt19937();
  rng.seed(Ion_channels::seed++);
  bin = boost::random::binomial_distribution<>(10, 0);
  uni = boost::random::uniform_01<>();
  stateCounterVec[0] = 0;
  stateCounterVec[1] = num_channels();
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
void Ion_channels::SetAsOpenState(modigliani_base::Size newOpenState) {
  open_states_.push_back(newOpenState);
}

void Ion_channels::reset() {
  stateCounterVec[0] = 0;
  stateCounterVec[1] = num_channels();
}

/**  */
modigliani_base::ReturnEnum Ion_channels::GillespieStep(
    modigliani_base::Real voltage) {
  if (!num_channels_) {
    return (modigliani_base::ReturnEnum::SUCCESS);
  }
  modigliani_base::Real val = uni(rng);
  modigliani_base::Real deltaT = timeStep();
  modigliani_base::Real channelTau = ComputeChannelStateTimeConstant(voltage);
  std::cerr << "channelTau=" << channelTau << std::endl;

  modigliani_base::Real sum = 0.0;
  for (modigliani_base::Size ll = 1; ll < num_states() + 1; ll++) {
    modigliani_base::Real stateChangeProbability = 0;
    for (modigliani_base::Size nextState = 1; nextState < num_states() + 1;
        nextState++) {
      if (ll == nextState) continue;
      stateChangeProbability += _probMatrix->GetTransitionProbability(
          voltage, ll, nextState);
    }
    sum += (stateChangeProbability / deltaT) * stateCounterVec[ll];
    std::cerr << "p=" << sum / (channelTau) << " val=" << val << std::endl;
    if (val < sum / channelTau) {
      return (ComputeGillespieStep(ll, voltage));
    }
  }
  std::cerr << "Ion_channels::GillespieStep - Error : "
            << "Control flow should not have reach here. "
            << "No channel transition done." << std::endl;
  return (modigliani_base::ReturnEnum::FAIL);
}

/**
 * Take a step using the single channel algorithm
 * @return
 */
modigliani_base::ReturnEnum Ion_channels::Step(modigliani_base::Real voltage) {
  if (!num_channels_) return (modigliani_base::ReturnEnum::SUCCESS);
  modigliani_base::Real rv = 0;
  std::vector<int> oldStateCounterVec = stateCounterVec;
  modigliani_base::Size matrix_index = _probMatrix->GetIndex(voltage);
  for (modigliani_base::Size current_state = 1;
      current_state < num_states() + 1; current_state++) {
    for (int llc = 1; llc < oldStateCounterVec[current_state] + 1; llc++) {
      rv = uni(rng);
      modigliani_base::Real accumulatedProb = 0;
      for (modigliani_base::Size nextState = 1; nextState < num_states() + 1;
          nextState++) {
        if (_probMatrix->GetTransitionProbability(matrix_index, current_state,
                                                  nextState) == 0) continue;
        if (nextState == current_state) continue;
        // This replaces awkward tNN probabilities
        accumulatedProb += _probMatrix->GetTransitionProbability(matrix_index,
                                                                 current_state,
                                                                 nextState);
        if (rv <= accumulatedProb && stateCounterVec[current_state] > 0) {
          // The state counter std::vector starts at 1
          stateCounterVec[nextState]++;
          stateCounterVec[current_state]--;

          // We have found our next state
          break;
        }
      }
    }
  }
  return (modigliani_base::ReturnEnum::SUCCESS);
}

void Ion_channels::ShowStates() const {
  for (modigliani_base::Size ll = 1; ll < num_states() + 1; ll++)
    std::cout << stateCounterVec[ll] << " ";
  std::cout << std::endl;
}

/*
 *
 */
modigliani_base::Size Ion_channels::NumOpen() const {
  if (!num_channels_) return (0u);
  modigliani_base::Size count = 0;
  for (auto state : open_states_) {
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
  modigliani_base::Real deltaT = timeStep();
  for (modigliani_base::Size ll = 1; ll < num_states() + 1; ll++) {
    modigliani_base::Real stateChangeProbability = 0;
    for (modigliani_base::Size nextState = 1; nextState < num_states() + 1;
        nextState++) {
      if (ll == nextState) continue;
      stateChangeProbability += _probMatrix->GetTransitionProbability(
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
modigliani_base::ReturnEnum Ion_channels::ComputeGillespieStep(
    modigliani_base::Size stateId, modigliani_base::Real voltage) {
  std::cerr << "NTBP_ion_channels_o::ComputeGillespieStep" << std::endl;
// int index = (voltage + 100) * 1000;
// This operation is costly. So we do it only once.
  modigliani_base::Size matrix_index = _probMatrix->GetIndex(voltage);

  modigliani_base::Size oldOpen = NumOpen();
  modigliani_base::Real deltaT = timeStep();

  modigliani_base::Real val = uni(rng) * timeStep();
// the probability has to be converted into a rate

  modigliani_base::Real stateChangeProbability = 0;
  for (modigliani_base::Size nextState = 1; nextState < num_states() + 1;
      nextState++) {
    if (stateId == nextState) continue;
    stateChangeProbability += _probMatrix->GetTransitionProbability(
        matrix_index, stateId, nextState);
  }
  modigliani_base::Real accumulatedProb = 0;
  stateChangeProbability = stateChangeProbability / deltaT;
  for (modigliani_base::Size nextState = 1; nextState < num_states() + 1;
      nextState++) {
    if (stateId == nextState
        || !_probMatrix->GetTransitionProbability(matrix_index, stateId,
                                                  nextState)) continue;

    accumulatedProb += _probMatrix->GetTransitionProbability(matrix_index,
                                                             stateId, nextState)
        / stateChangeProbability;

    // We want to ensure change in the state even
    // if float division induces errors
    if (val <= accumulatedProb || nextState == num_states() - 1) {
      // The state counter std::vector starts at 1
      stateCounterVec[nextState]++;
      stateCounterVec[stateId]--;
      break;
    }
  }

  /* CHECKING CODE */
  int check = 0.0;
  for (modigliani_base::Size ll = 1; ll < num_states() + 1; ll++) {
    check += stateCounterVec[ll];
    std::cerr << check << std::endl;
  }
  std::cerr << num_channels() << std::endl;
  M_ASSERT((unsigned int ) check == num_channels());

  if (NumOpen() == oldOpen) {
    return (modigliani_base::ReturnEnum::FAIL);
  } else {
    return (modigliani_base::ReturnEnum::SUCCESS);
  }
}

modigliani_base::ReturnEnum Ion_channels::BinomialStep(
    modigliani_base::Real voltage) {
  using modigliani_base::Size;
  if (!num_channels()) return (modigliani_base::ReturnEnum::SUCCESS);
  std::vector<int> newStateCounterVec = stateCounterVec;
// This operation is costly. So we do it only once.
  modigliani_base::Size matrix_index = _probMatrix->GetIndex(voltage);
  bool loop = false;
  modigliani_base::Size loopCounter = 0;

  do {
    loop = false;
    loopCounter++;
    for (modigliani_base::Size currentState = 1;
        currentState < num_states() + 1; currentState++) {
      for (modigliani_base::Size nextState = 1; nextState < num_states() + 1;
          nextState++) {
        if (nextState == currentState) continue;
        modigliani_base::Real prob = _probMatrix->GetTransitionProbability(
            matrix_index, currentState, nextState);
        if (prob == 0) continue;
        // M_ASSERT(prob>0 && prob<=1);
        int numberOfChannels = stateCounterVec[currentState];
        //modigliani_base::Real delta = binomRnd.Binomial(prob, numberOfChannels);
        bin.param(boost::random::binomial_distribution<>::param_type {
            numberOfChannels, prob });
        modigliani_base::Real delta = bin(rng);
        newStateCounterVec[nextState] += delta;
        newStateCounterVec[currentState] -= delta;
      }
    }

    /* CHECKING CODE */
    Size check = 0;
    for (modigliani_base::Size ll = 1; ll < num_states() + 1; ll++) {
      if (newStateCounterVec[ll] < 0) {
        loop = true;
        break;
      }
      check += newStateCounterVec[ll];
    }
    if (check != num_channels_) loop = true;
  } while ((true == loop) && (loopCounter < 100));
  if (loopCounter >= 100) {
    debug_print("%s\n", "ERROR: Binominal step loop counter limit reached.");
    return (modigliani_base::ReturnEnum::SUCCESS);
  }
  stateCounterVec = newStateCounterVec;
  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Ion_channels::DeterministicStep(
    const modigliani_base::Real voltage) {
  std::vector<int> newStateCounterVec = stateCounterVec;
// This operation is costly. So we do it only once.
  modigliani_base::Size matrix_index = _probMatrix->GetIndex(voltage);
  bool loop = false;
  modigliani_base::Size loopCounter = 0;

  do {
    loop = false;
    loopCounter++;
    for (modigliani_base::Size currentState = 1;
        currentState < num_states() + 1; currentState++) {
      for (modigliani_base::Size nextState = 1; nextState < num_states() + 1;
          nextState++) {
        if (nextState == currentState) continue;
        modigliani_base::Real prob = 0;
        try {
          prob = _probMatrix->GetTransitionProbability(matrix_index,
                                                       currentState, nextState);
        } catch (std::out_of_range &e) {
          std::cerr << e.what() << std::endl;
          return (modigliani_base::ReturnEnum::FAIL);
        }

        if (prob == 0) continue;
        int numberOfChannels = stateCounterVec[currentState];
        int delta = floor(prob * numberOfChannels);
        newStateCounterVec[nextState] += delta;
        newStateCounterVec[currentState] -= delta;
      }
    }

    /* CHECKING CODE */
    modigliani_base::Size check = 0;
    for (modigliani_base::Size ll = 1; ll < num_states() + 1; ll++) {
      if (newStateCounterVec[ll] < 0) loop = true;
      check += newStateCounterVec[ll];
    }
    if (check != num_channels()) loop = true;
  } while ((true == loop) && (loopCounter < 100));
  if (loopCounter >= 100) {
    std::cerr << "ERROR: deterministic step loop counter limit reached."
              << std::endl;
    return (modigliani_base::ReturnEnum::SUCCESS);
  }
  stateCounterVec = newStateCounterVec;
  return (modigliani_base::ReturnEnum::SUCCESS);
}

/** Do not call before rate constants were set. */
modigliani_base::ReturnEnum Ion_channels::SteadyStateDistribution(
    modigliani_base::Real voltage) {
// TODO(Ali) dirty hack ... make time step dependent
// and maybe compute directly state values
  for (modigliani_base::Size ll = 0; ll < 1000; ll++) {
    BinomialStep(voltage);  // much faster then having to wait for Step()
  }
  return (modigliani_base::ReturnEnum::SUCCESS);
}
}  // namespace modigliani_core
