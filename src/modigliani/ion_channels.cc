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

#include "modigliani/ion_channels.h"

namespace modigliani {

bool Ion_channels::seed_set_ = 0;

Ion_channels::Ion_channels(modigliani::Size numNewChannels,
                           modigliani::Size numNewStates,
                           Transition_rate_matrix* probMatrix,
                           modigliani::Real newTimeStep)
    : Object(), _probMatrix(probMatrix),
      num_states_(numNewStates),
      num_channels_(numNewChannels) {
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

void Ion_channels::SetAsOpenState(modigliani::Size newOpenState) {
  open_states_.push_back(newOpenState);
}

modigliani::ReturnEnum Ion_channels::GillespieStep(
    modigliani::Real voltage) {
  if (!num_channels_) {
    return (modigliani::ReturnEnum::SUCCESS);
  }
  modigliani::Real val = uni(rng);
  modigliani::Real deltaT = timestep();
  modigliani::Real channelTau = ComputeChannelStateTimeConstant(voltage);
  std::cerr << "channelTau=" << channelTau << std::endl;

  modigliani::Real sum = 0.0;
  for (modigliani::Size ll = 1; ll < num_states() + 1; ll++) {
    modigliani::Real stateChangeProbability = 0;
    for (modigliani::Size nextState = 1; nextState < num_states() + 1;
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
  return (modigliani::ReturnEnum::FAIL);
}

modigliani::ReturnEnum Ion_channels::SingleChannelStep(modigliani::Real voltage) {
  if (!num_channels_) return (modigliani::ReturnEnum::SUCCESS);
  modigliani::Real rv = 0;
  std::vector<int> oldStateCounterVec = stateCounterVec;
  modigliani::Size matrix_index = _probMatrix->GetIndex(voltage);
  for (modigliani::Size current_state = 1;
      current_state < num_states() + 1; current_state++) {
    for (int llc = 1; llc < oldStateCounterVec[current_state] + 1; llc++) {
      rv = uni(rng);
      modigliani::Real accumulatedProb = 0;
      for (modigliani::Size nextState = 1; nextState < num_states() + 1;
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
  return (modigliani::ReturnEnum::SUCCESS);
}

modigliani::Size Ion_channels::NumOpen() const {
  if (!num_channels_) return (0u);
  modigliani::Size count = 0;
  for (auto state : open_states_) {
    count += stateCounterVec[state];
  }
  return (count);
}

modigliani::Real Ion_channels::ComputeChannelStateTimeConstant(
    modigliani::Real voltage) const {
  std::cerr << "NTBP_ion_channels_o::ComputeChannelStateTimeConstant()"
            << std::endl;
  modigliani::Real sum = 0.0;
  modigliani::Real deltaT = timestep();
  for (modigliani::Size ll = 1; ll < num_states() + 1; ll++) {
    modigliani::Real stateChangeProbability = 0;
    for (modigliani::Size nextState = 1; nextState < num_states() + 1;
        nextState++) {
      if (ll == nextState) continue;
      stateChangeProbability += _probMatrix->GetTransitionProbability(
          voltage, ll, nextState);
    }
    sum += (stateChangeProbability / deltaT) * stateCounterVec[ll];
  }
  return (sum);
}

modigliani::ReturnEnum Ion_channels::ComputeGillespieStep(
    modigliani::Size stateId, modigliani::Real voltage) {
  std::cerr << "NTBP_ion_channels_o::ComputeGillespieStep" << std::endl;
// int index = (voltage + 100) * 1000;
// This operation is costly. So we do it only once.
  modigliani::Size matrix_index = _probMatrix->GetIndex(voltage);

  modigliani::Size oldOpen = NumOpen();
  modigliani::Real deltaT = timestep();

  modigliani::Real val = uni(rng) * timestep();
// the probability has to be converted into a rate

  modigliani::Real stateChangeProbability = 0;
  for (modigliani::Size nextState = 1; nextState < num_states() + 1;
      nextState++) {
    if (stateId == nextState) continue;
    stateChangeProbability += _probMatrix->GetTransitionProbability(
        matrix_index, stateId, nextState);
  }
  modigliani::Real accumulatedProb = 0;
  stateChangeProbability = stateChangeProbability / deltaT;
  for (modigliani::Size nextState = 1; nextState < num_states() + 1;
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
  for (modigliani::Size ll = 1; ll < num_states() + 1; ll++) {
    check += stateCounterVec[ll];
    std::cerr << check << std::endl;
  }
  std::cerr << num_channels() << std::endl;
  assert((unsigned int) check == num_channels());
  if (NumOpen() == oldOpen) {
    return (modigliani::ReturnEnum::FAIL);
  } else {
    return (modigliani::ReturnEnum::SUCCESS);
  }
}

modigliani::ReturnEnum Ion_channels::BinomialStep(
    modigliani::Real voltage) {
  using modigliani::Size;
  if (!num_channels()) return (modigliani::ReturnEnum::SUCCESS);
  std::vector<int> newStateCounterVec = stateCounterVec;
  // This operation is costly. So we do it only once.
  modigliani::Size matrix_index = _probMatrix->GetIndex(voltage);
  bool loop = false;
  modigliani::Size loopCounter = 0;

  do {
    loop = false;
    loopCounter++;
    for (modigliani::Size currentState = 1;
        currentState < num_states() + 1; currentState++) {
      for (modigliani::Size nextState = 1; nextState < num_states() + 1;
          nextState++) {
        if (nextState == currentState) continue;
        modigliani::Real prob = _probMatrix->GetTransitionProbability(
            matrix_index, currentState, nextState);
        if (prob == 0) continue;
        assert(prob>0 && prob<=1);
        int numberOfChannels = stateCounterVec[currentState];
        bin.param(boost::random::binomial_distribution<>::param_type {
            numberOfChannels, prob });
        modigliani::Real delta = bin(rng);
        newStateCounterVec[nextState] += delta;
        newStateCounterVec[currentState] -= delta;
      }
    }

    /* CHECKING CODE */
    Size check = 0;
    for (modigliani::Size ll = 1; ll < num_states() + 1; ll++) {
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
    return (modigliani::ReturnEnum::SUCCESS);
  }
  stateCounterVec = newStateCounterVec;
  return (modigliani::ReturnEnum::SUCCESS);
}

modigliani::ReturnEnum Ion_channels::DeterministicStep(
    const modigliani::Real voltage) {
  std::vector<int> newStateCounterVec = stateCounterVec;
  // This operation is costly. So we do it only once.
  modigliani::Size matrix_index = _probMatrix->GetIndex(voltage);
  bool loop = false;
  modigliani::Size loopCounter = 0;

  do {
    loop = false;
    loopCounter++;
    for (modigliani::Size currentState = 1;
        currentState < num_states() + 1; currentState++) {
      for (modigliani::Size nextState = 1; nextState < num_states() + 1;
          nextState++) {
        if (nextState == currentState) continue;
        modigliani::Real prob = 0;
        try {
          prob = _probMatrix->GetTransitionProbability(matrix_index,
                                                       currentState, nextState);
        } catch(const std::out_of_range &e) {
          std::cerr << e.what() << std::endl;
          return (modigliani::ReturnEnum::FAIL);
        }

        if (prob == 0) continue;
        int numberOfChannels = stateCounterVec[currentState];
        int delta = floor(prob * numberOfChannels);
        newStateCounterVec[nextState] += delta;
        newStateCounterVec[currentState] -= delta;
      }
    }

    /* CHECKING CODE */
    modigliani::Size check = 0;
    for (modigliani::Size ll = 1; ll < num_states() + 1; ll++) {
      if (newStateCounterVec[ll] < 0) loop = true;
      check += newStateCounterVec[ll];
    }
    if (check != num_channels()) loop = true;
  } while ((true == loop) && (loopCounter < 100));
  if (loopCounter >= 100) {
    std::cerr << "ERROR: deterministic step loop counter limit reached."
              << std::endl;
    return (modigliani::ReturnEnum::SUCCESS);
  }
  stateCounterVec = newStateCounterVec;
  return (modigliani::ReturnEnum::SUCCESS);
}

modigliani::ReturnEnum Ion_channels::SteadyStateDistribution(
    modigliani::Real voltage) {
  for (modigliani::Size ll = 0; ll < 1000; ll++) {
    BinomialStep(voltage);  // much faster then having to wait for Step()
  }
  return (modigliani::ReturnEnum::SUCCESS);
}
}  // namespace modigliani
