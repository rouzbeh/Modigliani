/**
 * @file object.h
 * @brief bio-physics related main declaration
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_OBJECT_H_
#define MODIGLIANI_MODIGLIANI_CORE_OBJECT_H_

#ifdef __INTEL_COMPILER
#define override
#endif

#include "modigliani/obj.h"
#include "modigliani/physical_constants.h"

/**
 * @namespace modigliani
 * @brief Core classes for Modigliani
 *
 * This namespace contains classes directly used for simulations.
 * Notably, it contains ion channels and membrane classes. This is the
 * core of modigliani.
 *
 */
namespace modigliani {

  /**
   * @brief Base timestep in [@f$\si{\milli\second}@f$]
   */
  const modigliani::Real TIMESTEP =  0.001;

  /**
   * @brief Common kinetic function types.
   * @deprecated
   */
  enum KineticFunctionType {
    CONSTANT, LINEAR, FERMI, GAUSSIAN
  };

  /**
   * @brief Current types
   */
  enum CurrentType {
    LEAK, IONIC, EXTERNAL
  };

  /**
   * @brief Stochastic simulation algortihms
   * @warning Not all are actually implemented and well tested. Stick
   * to BINOMIALPOPULATION or DETERMINISTIC.
   */
  enum StochasticType {
    DETERMINISTIC,
    GILLESPIE,
    LANGEVIN,
    SINGLECHANNEL,
    NOISYMEAN,
    BINOMIALPOPULATION
  };

  /**
   * @brief Base temperature in [@f$\si{celsius}@f$]
   */
  const modigliani::Real STD_TEMPERATURE = 6.3;

  /**
   * @brief Converts kinetics to be used at a different temperature
   * than the one at which they were recorded.
   * @param temp Simulation temperature in [@f$\si{\celsius}@f$]
   * @param baseTemp Recording temperature in [@f$\si{\celsius}@f$]
   * @param q10 The Q10 factor
   * @return Factor by which to multiply the kinetics
   */
  modigliani::Real TemperatureRateRelation(
      modigliani::Real temp,
      modigliani::Real baseTemp,
      modigliani::Real q10);

  /**
   * @brief Compute length constant of a passive cable
   * @param diameter Diameter in [@f$\si{\micro\meter}@f$]
   * @param rAxoplasmic Axoplasmic resistance in
   * [@f$\si{\ohm\centi\meter}@f$]
   * @param gLeak Leak conductance in [@f$\si{\milli\siemens\per\centi\meter\squared}@f$]
   * @return Length constant in [@f$\si{\micro\meter}@f$]
   */
  modigliani::Real LengthConstantPassiveCable(
      modigliani::Real diameter,
      modigliani::Real rAxoplasmic,
      modigliani::Real gLeak);

  /**
   * @brief Base class for all objects in modigliani
   */
  class Object : public modigliani::Obj {
   public:
    Object();
    Object(const Object & original) = delete;
    Object & operator=(const Object & right) = delete;
    virtual ~Object();
    
    /**
     * @brief Returns simulation timestep
     * @return Timestep in [@f$\si{\milli\second}@f$]
     */
    modigliani::Real timestep() const {
      return (timestep_);
    }
    
    /**
     * @brief Sets simulation timestep
     * @param newTimeStep Timestep in [@f$\si{\milli\second}@f$]
     */
    void set_timestep(modigliani::Real newTimeStep) {
      timestep_ = newTimeStep;
    }

  private:
    // TODO(Ali) TIMESTEP SHOULD BECOME STATIC! FOR GLOBAL CHANGES!
    modigliani::Real timestep_;  // time step size in msec;
};
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_CORE_OBJECT_H_
