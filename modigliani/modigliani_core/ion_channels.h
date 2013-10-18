/**
 * @file ion_channels.h
 * @brief Ion_channels class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Mohammad Ali Neishabouri
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_ION_CHANNELS_H_
#define MODIGLIANI_MODIGLIANI_CORE_ION_CHANNELS_H_

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/binomial_distribution.hpp>
#include <vector>
#include "modigliani_core/object.h"
#include "modigliani_core/transition_rate_matrix.h"

namespace modigliani_core {
/**
 * @brief Base class for all ion channels
 */
class Ion_channels : public Object {
  public:
/**
 * @brief Constructor
 * @param numNewChannels Number of channels
 * @param numNewStates Number of possible states for the channel
 * @param probMatrix The transition probability matrix
 * @param newTimeStep Simulation timestep in
 * [@f$\si{\milli\second}@f$]
 */
    Ion_channels(modigliani_base::Size numNewChannels,
                 modigliani_base::Size numNewStates,
                 Transition_rate_matrix* probMatrix,
                 modigliani_base::Real newTimeStep);

    explicit Ion_channels(
        const Ion_channels & original) = delete;
    Ion_channels & operator=(const Ion_channels & right) = delete;

    virtual ~Ion_channels();

/**
 * @brief Returns number of channels.
 * @return Number of ion channels
 */
    modigliani_base::Size num_channels() const {
      return (num_channels_);
    }

/**
 * @brief Returns number of states.
 * @return Number of states
 */
    modigliani_base::Size num_states() const {
      return (num_states_);
    }

/**
 * @brief Runs one step of the gillespie algorithm at the given
 * membrane potential
 * @param voltage Membrane potential in [@f$\si{\milli\volt}@f$]
 * @return Success or failure
 * @ingroup Simulation
 */
    modigliani_base::ReturnEnum GillespieStep(modigliani_base::Real voltage);

/**
 * @brief Defines the given state as "open".
 *
 * Channels that are in one of the open states are taken into account
 * to calculate the conductance.
 * @param stateId State number
 * @warning States are numbered [1..m]
 */
    void SetAsOpenState(modigliani_base::Size stateId);

/**
 * @brief Runs one step of the binomial algorithm at the given
 * membrane potential.
 * @param voltage Membrane potential in [@f$\si{\milli\volt}@f$]
 * @return Success or failure
 *  @ingroup Simulation
 */
    modigliani_base::ReturnEnum BinomialStep(modigliani_base::Real voltage);

/**
 * @brief Runs one step of the deterministic algorithm at the given
 * membrane potential.
 *
 * This function basically assumes that the number of channels
 * undergoing a transition is equal to the number of channels in the
 * departing state times the transition probability. This is in
 * general not useful, especially for a small number of channels.
 * @param voltage Membrane potential in [@f$\si{\milli\volt}@f$]
 * @return Success or failure
 * @warning This is rarely useful.
 * @ingroup Simulation
 */
    modigliani_base::ReturnEnum DeterministicStep(
        const modigliani_base::Real voltage);

/**
 * @brief Runs one step of the single channel algorithm at the given
 * membrane potential.
 * @param voltage Membrane potential in [@f$\si{\milli\volt}@f$]
 * @return Success or failure
 * @ingroup Simulation
 */
    modigliani_base::ReturnEnum SingleChannelStep(modigliani_base::Real voltage);

/**
 * @brief Returns number of channels in open states.
 * @return Number of channels
 */
    modigliani_base::Size NumOpen() const;

/**
 * @brief Returns number of channels in a given state
 * @return Number of channels
 * @warning States are numbered [1..m]
 */
    modigliani_base::Size NumChannelsInState(
        modigliani_base::Size state) const {
      return (stateCounterVec[state]);
    }

/**
 * @brief Returns number of channels in closed states.
 * @return Number of channels
 */    
    modigliani_base::Size NumClosed() const {
      return (num_channels() - NumOpen());
    }

/**
 * @brief Distributes channels in different states according to the
 * steady-state distribution at the given membrane potential
 *
 * This is done by running 1000 step of the binomial algorithm. This
 * is not the best way to do it. Ideally, this should be time
 * dependent, or maybe we could directly calculate the distribution?
 * @param voltage Membrane potential in [@f$\si{\milli\volt}@f$]
 * @return Success or failure
 * @warning Do not call before rate constants were set.
 */ 
    virtual modigliani_base::ReturnEnum SteadyStateDistribution(
        modigliani_base::Real voltage);

/**
 * @brief Returns sum of escape rates from current state.
 *
 * This is useful for the Gillespie algorithm.
 * @param voltage Membrane potential
 * @return Escape rate in [@f$\si{\per\second}@f$]
 * @ingroup Simulation
 */
    virtual modigliani_base::Real ComputeChannelStateTimeConstant(
        modigliani_base::Real voltage) const;

/**
 * @brief Is the seed for the RNG set?
 */
    static bool seed_set_;

    protected:
      std::vector<modigliani_base::Real> statePersistenceProbVec;
      std::vector<int> stateCounterVec;
      boost::random::mt19937 rng;
      boost::random::binomial_distribution<> bin;
      boost::random::uniform_01<> uni;

    private:
    modigliani_base::ReturnEnum ComputeGillespieStep(
        modigliani_base::Size channelStateId, modigliani_base::Real voltage);
    Transition_rate_matrix* _probMatrix;
    std::vector<modigliani_base::Size> open_states_;
    const modigliani_base::Size num_states_;
    const modigliani_base::Size num_channels_;
    unsigned int seed;
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_ION_CHANNELS_H_
