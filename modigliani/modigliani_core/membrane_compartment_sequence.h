/**
 * @file membrane_compartment_sequence.h
 * @brief Membrane_compartment_sequence class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2011,2012,2013 Mohammad Ali Neishabouri
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_COMPARTMENT_SEQUENCE_H_
#define MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_COMPARTMENT_SEQUENCE_H_
#include <tnt/tnt_cmat.h>
#include <vector>
#include <fstream>
#include "modigliani_core/membrane.h"
#include "modigliani_core/cylindrical_compartment.h"

namespace modigliani_core {
/**
 * @class Membrane_compartment_sequence
 * @brief Sequence of cylindrical membrane compartments, basically a cable
 *
 * This class provides an easy way to arrange compartments
 * linearly, i.e. to simulate axons or dendrites. The class
 * takes care of propagating the membrane potential along
 * its member compartments.
 */
  class Membrane_compartment_sequence:public Membrane {
  public:
/**
 *  @brief Constructor
 *
 * Initialises variables, and sets the seed according to the current time.
 */
    Membrane_compartment_sequence();
    Membrane_compartment_sequence(const Membrane_compartment_sequence &
                                  original) = delete;
    const Membrane_compartment_sequence
        & operator=(const
                    Membrane_compartment_sequence
                    & right) = delete;
     virtual ~Membrane_compartment_sequence();

/**
 * @brief Adds a compartment to the axon.
 *
 * @param      compartPtr Pointer to the compartment to add
 * @return     Whether the compartment was added successfully
 * @warning    no update of SOLVER dimensionality or SOVLER INIT done
 */
     modigliani_base::ReturnEnum PushBack(Cylindrical_compartment * compartPtr);

/**
   @brief Initialises the propagation compartment sequence

   The most important operation here is to initialise the propagation
   matrix used at every step. This uses a technique developed in
   Aldo's PhD thesis. It accelerates calculations but seems to require
   a constant diameter.
   @warning    Constant axon diameter required
 */
     modigliani_base::ReturnEnum Init();

/**
 * @brief Setup staggering PDE integration of compartments
 *
 * Internal - voltage related - states of compartments(i.e. currents)
 * are ahead t+.5 baseTimeStep, while state of compartment sequence
 * is unchanged. imposes crank nicholson staggering.
 *
 * @warning    Calling method activates Crank-Nicholson algorithm in Step()
 */
     modigliani_base::ReturnEnum InitialStep();

/**
 * @brief Execute one time step on the compartments.
 * @warning Identical axo-geometric properties required for all compartments !
 */
     modigliani_base::ReturnEnum Step();

/**
 * @brief Inject current into a compartment
 *
 * This is very useful in all sorts of experiments. Usually the
 * current is injected into the first compartment (1).
 * @param current is nano amperes
 * @param compartmentId refers to intuitive enumeriation, i.e. [1..m]
 * @return Success state
 * @warning Compartments numbered [1..m]
 */
    modigliani_base::ReturnEnum InjectCurrent(modigliani_base::
                                              Real current /* in nA */ ,
                                              modigliani_base::
                                              Size compartmentId);

/**
 * @brief Return the number of compartments
 *
 * @return Number of compartments
 */   
    modigliani_base::Size num_compartments()const {
      return (num_compartments_);
    }

/**
 * @brief Compute sum of escape rates over current state
 *
 * This is simply the sum of time constants of all compartments
 * @return Time constant in [kHz]
 */  
    modigliani_base::Real CompartmentSequenceChannelStateTimeConstant() const;

/**
 * @brief Returns the amount of current for the given index in the
 * compartment
 * @param compIndex compartment index
 * @param currIndex current index
 * @return Current
 * @warning Currents indexed [1..m]
 */   
    modigliani_base::Real AttachedCurrent(modigliani_base::Size compIndex,
                                          modigliani_base::Size currIndex) {
      assert(compIndex > 0);
      return (compartment_vec_[compIndex - 1]->Current(currIndex)->current());
    }

/**
 * @brief Returns the requested compartment
 * @param index compartment index
 * @return Pointer to the requested compartment
 * @warning Currents indexed [1..m]
 */     
    Cylindrical_compartment *ReturnCompartment(modigliani_base::Size index);

/**
 * @brief Run a Gillespie step
 * @return Success
 * @warning Untested by Ali
 */    
    modigliani_base::ReturnEnum GillespieStep();

/**
 * @brief Holds pointers to member compartments
 */     
    std::vector < Cylindrical_compartment * > compartment_vec_;

    static bool seed_set_;

  protected:

/**
 * @brief Solves a tri-diagonal matrix system
 *
 * The algorithm is taken from the book numerical receipes
 * @param l Lower diagonal
 * @param d Diagonal
 * @param u Upper diagonal
 * @param r Right hand side
 * @return Solution vector
 */
    std::vector < modigliani_base::Real >
      NumericalRecipesSolveTriDiag(const std::vector < modigliani_base::Real >
                                   &l,
                                   const std::vector < modigliani_base::Real >
                                   &d,
                                   const std::vector < modigliani_base::Real >
                                   &u,
                                   const std::vector < modigliani_base::Real >
                                   &r) const;

  private:
    modigliani_base::Real _sigma(const Cylindrical_compartment * from,
                                   const Cylindrical_compartment * to) const;

    std::vector < modigliani_base::Real > l_vec_;
    std::vector < modigliani_base::Real > d_vec_;
    std::vector < modigliani_base::Real > u_vec_;
    std::vector < modigliani_base::Real > r_vec_;
    
    modigliani_base::Size num_compartments_;
    bool initialised_;
    bool sw_crank_nicholson_;

    boost::random::mt19937 rng_;
    boost::random::binomial_distribution <> bin_;
    boost::random::uniform_01 <> uni_;
    unsigned int seed_;
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_COMPARTMENT_SEQUENCE_H_
