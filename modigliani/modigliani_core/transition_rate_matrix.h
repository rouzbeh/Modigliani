/**
 * @file transition_rate_matrix.h
 * Transition_rate_matrix class declaration
 * @author Ali Neishabouri <ali@neishabouri.net>
 * Copyright 2011 Ali Neishabouri
 */

#ifndef MODIGLIANI_MODIGLIANI_CORE_TRANSITION_RATE_MATRIX_H_
#define MODIGLIANI_MODIGLIANI_CORE_TRANSITION_RATE_MATRIX_H_

#include <cstdlib>
#include <stdexcept>
#include "modigliani_base/types.h"
#include "modigliani_core/object.h"

namespace modigliani_core {
/**
  * @class Transition_rate_matrix
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
    Transition_rate_matrix(modigliani_base::Size numNewStates,
                           modigliani_base::Real min, modigliani_base::Real max,
                           modigliani_base::Real step);
    virtual ~Transition_rate_matrix();

/**
 * @brief Sets the transition probability for a given membrane potential
 *
 * @param voltage membrane potential
 * @param start starting state
 * @param stop destination state
 * @param probability The transition probability
 */    
    void SetTransitionProbability(modigliani_base::Real voltage,
                                  modigliani_base::Size start,
                                  modigliani_base::Size stop,
                                  modigliani_base::Real probability);

/**
 * @brief Sets the transition probability for a given index
 *
 * The index is the index in the 1 dimensional array.
 * @param index Index
 * @param start starting state
 * @param stop destination state
 * @param probability The transition probability
 */     
    void SetTransitionProbability(modigliani_base::Size index,
                                  modigliani_base::Size start,
                                  modigliani_base::Size stop,
                                  modigliani_base::Real probability);

/**
 * @brief Returns the transition probability for a given membrane potential
 *
 * @param voltage The membrane potential
 * @param start starting state
 * @param stop destination state
 * @return The transition probability
 */       
     modigliani_base::Real GetTransitionProbability(modigliani_base::
                                                    Real voltage,
                                                    modigliani_base::Size start,
                                                    modigliani_base::Size stop);

/**
 * @brief Returns the transition probability for a given index
 *
 * The index is the index in the 1 dimensional array.
 * @param index Index
 * @param start starting state
 * @param stop destination state
 * @return The transition probability
 */      
     modigliani_base::Real GetTransitionProbability(modigliani_base::Size index,
                                                    modigliani_base::Size start,
                                                    modigliani_base::Size stop);

/**
 * @brief Returns the index for a given membrane potential
 *
 * The index is the index in the 1 dimensional array.
 * @param voltage The membrane potential
 * @return The index
 */        
     modigliani_base::Size GetIndex(modigliani_base::Real voltage);

  private:
    typedef modigliani_base::Real * matrix_array_t;
    matrix_array_t prob_matrices_;

     modigliani_base::Real min_;
     modigliani_base::Real max_;
     modigliani_base::Real step_;
    int num_states_;
     modigliani_base::Size max_index_;
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_TRANSITION_RATE_MATRIX_H_
