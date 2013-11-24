/**
 * @file ion_channels.cc
 * @brief Ion_channels class implementation
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * Copyright (C) 2013 Mohammad Ali Neishabouri
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modigliani_core/ion_channels.h"

namespace modigliani_core {

bool Ion_channels::seed_set_ = 0;

Ion_channels::Ion_channels(modigliani_base::Size numNewChannels,
                           modigliani_base::Size numNewStates,
                           Transition_rate_matrix* probMatrix,
                           modigliani_base::Real newTimeStep)
    : Object(), _probMatrix(probMatrix),
      num_channels_(numNewChannels), num_states_(
        numNewStates) {
  set_timestep(newTimeStep);
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

Ion_channels::~Ion_channels() {
}

void Ion_channels::SetAsOpenState(modigliani_base::Size newOpenState) {
  open_states_.push_back(newOpenState);
}

modigliani_base::ReturnEnum Ion_channels::GillespieStep(
    modigliani_base::Real voltage) {
  if (!num_channels_) {
    return (modigliani_base::ReturnEnum::SUCCESS);
  }
  modigliani_base::Real val = uni(rng);
  modigliani_base::Real deltaT = timestep();
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

modigliani_base::ReturnEnum Ion_channels::SingleChannelStep(modigliani_base::Real voltage) {
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

modigliani_base::Size Ion_channels::NumOpen() const {
  if (!num_channels_) return (0u);
  modigliani_base::Size count = 0;
  for (auto state : open_states_) {
    count += stateCounterVec[state];
  }
  return (count);
}

modigliani_base::Real Ion_channels::ComputeChannelStateTimeConstant(
    modigliani_base::Real voltage) const {
  std::cerr << "NTBP_ion_channels_o::ComputeChannelStateTimeConstant()"
            << std::endl;
  modigliani_base::Real sum = 0.0;
  modigliani_base::Real deltaT = timestep();
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

modigliani_base::ReturnEnum Ion_channels::ComputeGillespieStep(
    modigliani_base::Size stateId, modigliani_base::Real voltage) {
  std::cerr << "NTBP_ion_channels_o::ComputeGillespieStep" << std::endl;
// int index = (voltage + 100) * 1000;
// This operation is costly. So we do it only once.
  modigliani_base::Size matrix_index = _probMatrix->GetIndex(voltage);

  modigliani_base::Size oldOpen = NumOpen();
  modigliani_base::Real deltaT = timestep();

  modigliani_base::Real val = uni(rng) * timestep();
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
  assert((unsigned int) check == num_channels());
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
        assert(prob>0 && prob<=1);
        int numberOfChannels = stateCounterVec[currentState];
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
    std::cerr
        << "ERROR: Binominal step loop counter limit reached."
        << std::endl;
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
        } catch(const std::out_of_range &e) {
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

modigliani_base::ReturnEnum Ion_channels::SteadyStateDistribution(
    modigliani_base::Real voltage) {
  for (modigliani_base::Size ll = 0; ll < 1000; ll++) {
    BinomialStep(voltage);  // much faster then having to wait for Step()
  }
  return (modigliani_base::ReturnEnum::SUCCESS);
}
}  // namespace modigliani_core
