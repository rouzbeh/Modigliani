/**
 * @file transition_rate_matrix.h
 * @brief Transition_rate_matrix class header
 *
 * Copyright 2011 Ali Neishabouri
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Modigliani is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODIGLIANI_MODIGLIANI_CORE_TRANSITION_RATE_MATRIX_H_
#define MODIGLIANI_MODIGLIANI_CORE_TRANSITION_RATE_MATRIX_H_

#include <cstdlib>
#include <stdexcept>
#include "modigliani/types.h"
#include "modigliani/object.h"

namespace modigliani {
/**
  * @class Transition_rate_matrix
  * @brief Holds voltage dependent transition rates for a Markov chain
  *
  * This classes goal is to hold the transition rates for all pairs of
  * states. Transition rates are stored independently for a range of
  * membrane potentials. This range and the precision are specified to
  * the constructor.
  *
  * The reason we use this class is because Boost's
  * multi array class can be slow, if the data is not accessed properly.
  * See
  * http://stackoverflow.com/questions/446866/boostmulti-array-performance-question.
  *
  * Here we actually only use a 1 dimensional C-array, but present it
  * as a 3-dim matrix.
*/
  class Transition_rate_matrix:public Object {
  public:
/**
 * @brief Constructor
 *
 * @param numNewStates Number of new states
 * @param min Minimum voltage
 * @param max Maximum voltage
 * @param step Voltage step
 */
    Transition_rate_matrix(modigliani::Size numNewStates,
                           modigliani::Real min, modigliani::Real max,
                           modigliani::Real step);
    virtual ~Transition_rate_matrix();

/**
 * @brief Sets the transition probability for a given membrane potential
 *
 * @param voltage membrane potential
 * @param start starting state
 * @param stop destination state
 * @param probability The transition probability
 */    
    void SetTransitionProbability(modigliani::Real voltage,
                                  modigliani::Size start,
                                  modigliani::Size stop,
                                  modigliani::Real probability);

/**
 * @brief Sets the transition probability for a given index
 *
 * The index is the index in the 1 dimensional array.
 * @param index Index
 * @param start starting state
 * @param stop destination state
 * @param probability The transition probability
 */     
    void SetTransitionProbability(modigliani::Size index,
                                  modigliani::Size start,
                                  modigliani::Size stop,
                                  modigliani::Real probability);

/**
 * @brief Returns the transition probability for a given membrane potential
 *
 * @param voltage The membrane potential
 * @param start starting state
 * @param stop destination state
 * @return The transition probability
 */       
     modigliani::Real GetTransitionProbability(modigliani::
                                                    Real voltage,
                                                    modigliani::Size start,
                                                    modigliani::Size stop);

/**
 * @brief Returns the transition probability for a given index
 *
 * The index is the index in the 1 dimensional array.
 * @param index Index
 * @param start starting state
 * @param stop destination state
 * @return The transition probability
 */      
     modigliani::Real GetTransitionProbability(modigliani::Size index,
                                                    modigliani::Size start,
                                                    modigliani::Size stop);

/**
 * @brief Returns the index for a given membrane potential
 *
 * The index is the index in the 1 dimensional array.
 * @param voltage The membrane potential
 * @return The index
 */        
     modigliani::Size GetIndex(modigliani::Real voltage);

  private:
    typedef modigliani::Real * matrix_array_t;
    matrix_array_t prob_matrices_;

     modigliani::Real min_;
     modigliani::Real max_;
     modigliani::Real step_;
    int num_states_;
     modigliani::Size max_index_;
  };
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_CORE_TRANSITION_RATE_MATRIX_H_
