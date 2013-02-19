/**
 * @file object.h
 * bio-physics related main declaration
 * @author Ahmed Aldo Faisal &copy; created 15.3.2001
 * NetTrader - visualisation, scientific and financial analysis and simulation system
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

/**
 * @namespace modigliani_core
 * @short Core classes
 *
 * This namespace contains classes directly used for simulations.
 * Notably, it contains ion channels and membrane classes.
 *
 */

#ifndef _Object_h_
#define _Object_h_

#define NTBP_TIMESTEP 0.001 /* in msec */

/* NT includes */
#include "../modigliani_base/error.h"
/* other includes */
#include "../modigliani_base/physical_constants.h"

/** @short Object class
 \bug unknown
 \warning unknown
 TIMESTEP SHOULD BECOME STATIC! FOR GLOBAL CHANGES!
 */

namespace modigliani_core {

enum NTBPKineticFunctionType {
  NTBP_CONSTANT, NTBP_LINEAR, NTBP, NTBP_FERMI, NTBP_GAUSSIAN
};
enum NTBPcurrentType {
  NTBP_LEAK, NTBP_IONIC, NTBP_EXTERNAL
};
enum StochasticType {
  DETERMINISTIC,
  GILLESPIE,
  LANGEVIN,
  SINGLECHANNEL,
  NOISYMEAN,
  BINOMIALPOPULATION
};

const modigliani_base::Real NTBP_STD_TEMPERATURE = 6.3;  // Celsius (NOT TO BE CHANGED)

modigliani_base::Real NTBP_temperature_rate_relation(
    modigliani_base::Real temp /* C */, modigliani_base::Real q10);
modigliani_base::Real TemperatureRateRelation(
    modigliani_base::Real temp /* C */, modigliani_base::Real baseTemp /* C */,
    modigliani_base::Real q10);
/** Compute length constant in muMeter */
modigliani_base::Real NTBP_length_constant_passive_cable(
    modigliani_base::Real diameter /* mu */,
    modigliani_base::Real rAxoplasmic /* Ohm cm */,
    modigliani_base::Real gLeak /* mSiemens/cm^2 */);

class Object : public modigliani_base::Obj {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Object();
    Object(const Object & original);
    Object & operator=(const Object & right);
    virtual ~Object();
    /* ***  Methods              ***/
    /** @short  update of explicitly time dependent variables using a deltaT = timeStep, NO update
     of dependent variables
     @param      none
     @return     none
     \warning    indirectly time dependent variables NOT updated
     \bug        unknown
     */
    modigliani_base::Real update_timeStep(
        modigliani_base::Real newSuggestedTimeStep) { /* if (suggestedTimeStep > newSuggestedTimeStep) */
      suggestedTimeStep = newSuggestedTimeStep;
      return (newSuggestedTimeStep);
    }
    virtual modigliani_base::ReturnEnum Step() {
      std::cerr
          << "Object:Step() - Warning : Non-overriden base method () called by derived class."
          << std::endl;
      return (modigliani_base::ReturnEnum::NOT_IMPLEMENTED);
    }
    modigliani_base::ReturnEnum StepNTBP() {
      timeStep_ = suggestedTimeStep;
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    modigliani_base::Real timeStep() const {
      return (timeStep_);
    }
    modigliani_base::Real _baseTimeStep() const {
      return (baseTimeStep);
    }
    void setTimeStep(modigliani_base::Real newTimeStep) {
      timeStep_ = newTimeStep;
    }
    /* ***  Data                 ***/
  protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
  private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    modigliani_base::Real baseTimeStep;  // time step size in msec;
    static modigliani_base::Real timeStep_;  // time step size in msec;
    static modigliani_base::Real suggestedTimeStep;  // time step size in msec;
};
}
#endif /* _Object.h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
