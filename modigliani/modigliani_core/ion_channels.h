/**
 * @file ion_channels.h
 * Ion_channels class header
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_ION_CHANNELS_H_
#define MODIGLIANI_MODIGLIANI_CORE_ION_CHANNELS_H_

#include <vector>
#include <ostream>

/* Parent includes */
#include "modigliani_core/object.h"
/* NT includes */
#include "modigliani_base/uniform_rnd_dist.h"
#include "modigliani_base/binomial_rnd_dist.h"
#include "modigliani_core/transition_rate_matrix.h"
/* other includes */

namespace modigliani_core {
/**
 * @short Ion_channels class
 * \bug unknown
 * \warning unknown
 */
class Ion_channels : public Object {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Ion_channels(modigliani_base::Size numNewChannels,
                 modigliani_base::Size numNewStates,
                 Transition_rate_matrix* probMatrix,
                 modigliani_base::Real newTimeStep = 0.1);
    explicit Ion_channels(const Ion_channels & original);
    Ion_channels & operator=(const Ion_channels & right);
    virtual ~Ion_channels();
    /* ***  Methods              ***/
    modigliani_base::Size num_channels() const {
      return (num_channels_);
    }
    modigliani_base::Size num_states() const {
      return (num_states_);
    }
    modigliani_base::ReturnEnum GillespieStep(modigliani_base::Real voltage);
    void setAsOpenState(modigliani_base::Size newOpenState);
    modigliani_base::ReturnEnum BinomialStep(modigliani_base::Real voltage);
    modigliani_base::ReturnEnum DeterministicStep(
        modigliani_base::Real voltage);
    modigliani_base::ReturnEnum Step(modigliani_base::Real voltage);
    void ShowStates() const;
    modigliani_base::Size NumOpen() const;
    modigliani_base::Size numChannelsInState(
        modigliani_base::Size state) const {
      return (stateCounterVec[state]);
    }
    modigliani_base::Size NumClosed() const {
      return (num_channels() - NumOpen());
    }
    virtual modigliani_base::ReturnEnum SteadyStateDistribution(
        modigliani_base::Real voltage);
    /** Sum of escape rates from current state [1/s] */
    virtual modigliani_base::Real ComputeChannelStateTimeConstant(
        modigliani_base::Real voltage) const;

  protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    // TODO(Ali) remove this
    std::vector<modigliani_base::Real> statePersistenceProbVec;
    std::vector<int> stateCounterVec;
    // [0,1] random number generation
    static modigliani_base::Uniform_rnd_dist uniformRnd;
    static modigliani_base::Binomial_rnd_dist binomRnd;

  private:
    /* ***  Methods              ***/
    modigliani_base::ReturnEnum ComputeGillespieStep(
        modigliani_base::Size channelStateId, modigliani_base::Real voltage);
    /* ***  Data                 ***/
    Transition_rate_matrix* _probMatrix;
    std::vector<modigliani_base::Size> openStates;
    modigliani_base::Size num_states_;
    modigliani_base::Size num_channels_;
};
}
#endif  // MODIGLIANI_MODIGLIANI_CORE_ION_CHANNELS_H_
